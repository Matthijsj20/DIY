#!/usr/bin/env python3
"""Merge per-project compile_commands.json and map shared/ sources for clangd."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PROJECTS = ("bell", "buzzer")
OUTPUT = ROOT / "compile_commands.json"


def load_db(path: Path) -> list[dict]:
    if not path.is_file():
        print(f"warning: missing {path}", file=sys.stderr)
        return []
    with path.open(encoding="utf-8") as handle:
        return json.load(handle)


def normalize(entry: dict) -> dict:
    directory = Path(entry["directory"]).resolve()
    file_path = Path(entry["file"])
    if not file_path.is_absolute():
        file_path = (directory / file_path).resolve()
    return {**entry, "directory": str(directory), "file": str(file_path)}


def dedupe_key(entry: dict) -> str:
    return entry["file"]


def shared_source_entries(project: str, entries: list[dict]) -> list[dict]:
    project_dir = ROOT / project
    shared_src = ROOT / "shared" / "src"
    if not shared_src.is_dir():
        return []

    lib_pattern = re.compile(
        rf"{re.escape(str(project_dir))}/\.pio/libdeps/[^/]+/doorbell-shared/src/([^/]+\.cpp)$"
    )
    templates: dict[str, dict] = {}
    for entry in entries:
        match = lib_pattern.match(entry["file"])
        if match:
            templates[match.group(1)] = entry

    merged: list[dict] = []
    for source in sorted(shared_src.glob("*.cpp")):
        template = templates.get(source.name)
        if template is None:
            continue
        command = template["command"]
        lib_source = template["file"]
        shared_source = str(source)
        if lib_source in command:
            command = command.replace(lib_source, shared_source)
        else:
            lib_name = Path(lib_source).name
            command = re.sub(rf"(?<=\s){re.escape(lib_name)}(?=\s*$)", shared_source, command)
        merged.append(
            {
                "directory": str(project_dir.resolve()),
                "file": shared_source,
                "command": command,
            }
        )
    return merged


def main() -> int:
    merged: dict[str, dict] = {}

    for project in PROJECTS:
        project_db = [normalize(entry) for entry in load_db(ROOT / project / "compile_commands.json")]
        for entry in project_db:
            merged[dedupe_key(entry)] = entry
        for entry in shared_source_entries(project, project_db):
            merged.setdefault(dedupe_key(entry), entry)

    output = sorted(merged.values(), key=lambda entry: entry["file"])
    with OUTPUT.open("w", encoding="utf-8") as handle:
        json.dump(output, handle, indent=2)
        handle.write("\n")

    print(f"Wrote {len(output)} entries to {OUTPUT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
