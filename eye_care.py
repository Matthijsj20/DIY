import rumps
from threading import Timer
import time
from mac_notifications import client
import AppKit
import os
import json


DEFAULT_WORK = 10
DEFAULT_RELAX = 5

APP_NAME = "EyeCareApp"
CONFIG_DIR = os.path.expanduser(f"~/Library/Application Support/{APP_NAME}")
CONFIG_FILE = os.path.join(CONFIG_DIR, "settings.json")


class EyeCareApp(rumps.App):

    def __init__(self):
        super().__init__("👁️")

        self.state = "Work"

        self.timer = None
        self.end_time = None
        self.remaining_time = None

        self.pending_event = None
        self.paused = False

        # load settings FIRST
        self.load_settings()

        self.start_action = rumps.MenuItem("Start Relax", callback=self.start_action_clicked)
        self.pause_action = rumps.MenuItem("Pause", callback=self.pause_resume)

        self.menu = [
            self.start_action,
            self.pause_action,
            "Set Work Time",
            "Set Relax Time",
            "State"
        ]

        self.start_work()

        self.ui_timer = rumps.Timer(self.update_ui, 1)
        self.ui_timer.start()

        self.event_timer = rumps.Timer(self.handle_event, 0.2)
        self.event_timer.start()

    # -----------------------------
    # SETTINGS STORAGE
    # -----------------------------

    def load_settings(self):
        try:
            with open(CONFIG_FILE, "r") as f:
                data = json.load(f)

            self.work_time = data.get("work_time", DEFAULT_WORK)
            self.relax_time = data.get("relax_time", DEFAULT_RELAX)

        except FileNotFoundError:
            self.work_time = DEFAULT_WORK
            self.relax_time = DEFAULT_RELAX

        except Exception:
            self.work_time = DEFAULT_WORK
            self.relax_time = DEFAULT_RELAX

    def save_settings(self):
        os.makedirs(CONFIG_DIR, exist_ok=True)

        data = {
            "work_time": self.work_time,
            "relax_time": self.relax_time
        }

        with open(CONFIG_FILE, "w") as f:
            json.dump(data, f, indent=2)

    # -----------------------------
    # HELPERS
    # -----------------------------

    def bring_to_front(self):
        AppKit.NSApplication.sharedApplication().activateIgnoringOtherApps_(True)

    def format_mmss(self, seconds):
        m, s = divmod(seconds, 60)
        return f"{m:02d}:{s:02d}"

    def parse_mmss(self, text):
        text = text.strip()

        if ":" in text:
            m, s = text.split(":")
            return int(m) * 60 + int(s)

        return int(text) * 60

    # -----------------------------
    # TIMER CORE
    # -----------------------------

    def schedule(self, seconds, event):
        if self.timer:
            self.timer.cancel()

        self.end_time = time.time() + seconds

        self.timer = Timer(seconds, self.set_event, args=(event,))
        self.timer.start()

    def set_event(self, event):
        self.pending_event = event

    # -----------------------------
    # STATE CONTROL
    # -----------------------------

    def start_work(self):
        self.state = "Work"
        self.paused = False
        self.schedule(self.work_time, "work_done")
        self.update_start_button()
        self.update_pause_button()

    def start_relax(self):
        self.state = "Relax"
        self.paused = False
        self.schedule(self.relax_time, "relax_done")
        self.update_start_button()
        self.update_pause_button()

    # -----------------------------
    # PAUSE / RESUME
    # -----------------------------

    def pause_resume(self, _):
        if not self.paused:
            if self.timer:
                self.timer.cancel()

            self.remaining_time = max(0, int(self.end_time - time.time()))
            self.paused = True

        else:
            self.paused = False

            if self.state == "Work":
                self.schedule(self.remaining_time, "work_done")
            else:
                self.schedule(self.remaining_time, "relax_done")

        self.update_pause_button()

    def update_pause_button(self):
        self.pause_action.title = "Resume" if self.paused else "Pause"

    # -----------------------------
    # EVENT LOOP
    # -----------------------------

    def handle_event(self, _):
        if not self.pending_event:
            return

        event = self.pending_event
        self.pending_event = None

        if event == "work_done":
            self.show_break_notification()

        elif event == "relax_done":
            client.create_notification(
                title="Back to work",
                subtitle="Focus time"
            )
            self.start_work()

    # -----------------------------
    # NOTIFICATIONS
    # -----------------------------

    def show_break_notification(self):
        client.create_notification(
            title="Eye break",
            subtitle="Click Start Relax",
            action_button_str="Start relax",
            action_callback=self.start_relax_from_notification
        )

    def start_relax_from_notification(self, _=None):
        self.start_relax()

    # -----------------------------
    # UI
    # -----------------------------

    def update_ui(self, _):
        if not self.end_time:
            return

        if self.paused:
            remaining = self.remaining_time
        else:
            remaining = int(self.end_time - time.time())

        m, s = divmod(max(0, remaining), 60)
        self.title = f"{self.state} {m:02d}:{s:02d}"

        self.menu["State"].title = f"State: {self.state}"

    # -----------------------------
    # MENU SWITCH BUTTON
    # -----------------------------

    def update_start_button(self):
        if self.state == "Work":
            self.start_action.title = "Start Relax"
        else:
            self.start_action.title = "Start Work"

    def start_action_clicked(self, _):
        if self.state == "Work":
            self.start_relax()
        else:
            self.start_work()

    # -----------------------------
    # SETTINGS UI
    # -----------------------------

    @rumps.clicked("Set Work Time")
    def set_work_time(self, _):
        self.bring_to_front()

        response = rumps.Window(
            title="Work time",
            message="Enter time (MM:SS)",
            default_text=self.format_mmss(self.work_time),
            ok="Set",
            cancel="Cancel"
        ).run()

        if response.clicked:
            try:
                self.work_time = self.parse_mmss(response.text)
                self.save_settings()
            except:
                rumps.alert("Invalid format")

    @rumps.clicked("Set Relax Time")
    def set_relax_time(self, _):
        self.bring_to_front()

        response = rumps.Window(
            title="Relax time",
            message="Enter time (MM:SS)",
            default_text=self.format_mmss(self.relax_time),
            ok="Set",
            cancel="Cancel"
        ).run()

        if response.clicked:
            try:
                self.relax_time = self.parse_mmss(response.text)
                self.save_settings()
            except:
                rumps.alert("Invalid format")


if __name__ == "__main__":
    EyeCareApp().run()