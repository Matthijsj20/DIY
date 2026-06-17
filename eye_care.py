import rumps
from threading import Timer
import time
from mac_notifications import client


WORK_TIME = 10   # shorten for debugging first
RELAX_TIME = 5


class EyeCareApp(rumps.App):

    def __init__(self):
        super().__init__("👁️")

        self.state = "WORK"
        self.timer = None
        self.end_time = None

        self.pending_event = None

        self.menu = [
            "Start Relax",
            "Pause",
            "State"
        ]

        self.start_work()

        self.ui_timer = rumps.Timer(self.update_ui, 1)
        self.ui_timer.start()

        # VERY IMPORTANT: event pump (this is the missing piece before)
        self.event_timer = rumps.Timer(self.handle_event, 0.2)
        self.event_timer.start()

    # -----------------------------
    # TIMER LOGIC (THREAD SAFE)
    # -----------------------------

    def start_work(self):
        self.state = "WORK"
        self.end_time = time.time() + WORK_TIME
        self.schedule(WORK_TIME, "work_done")

    def start_relax(self):
        self.state = "RELAX"
        self.end_time = time.time() + RELAX_TIME
        self.schedule(RELAX_TIME, "relax_done")

    def relax_done(self):
        self.pending_event = "relax_done"

    def work_done(self):
        self.pending_event = "work_done"

    def schedule(self, seconds, event):
        if self.timer:
            self.timer.cancel()

        self.timer = Timer(seconds, self.set_event, args=(event,))
        self.timer.start()

    def set_event(self, event):
        self.pending_event = event

    # -----------------------------
    # MAIN THREAD EVENT HANDLER
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
                subtitle="Focus time",
                action_button_str="Start work",
                action_callback=self.start_work_from_notification
            )

            self.start_work()

    # -----------------------------
    # NOTIFICATION
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

    def start_work_from_notification(self, _=None):
        self.start_work()

    # -----------------------------
    # UI
    # -----------------------------

    def update_ui(self, _):
        if not self.end_time:
            return

        remaining = int(self.end_time - time.time())
        m, s = divmod(max(0, remaining), 60)

        self.title = f"{self.state} {m:02d}:{s:02d}"
        self.menu["State"].title = f"State: {self.state}"

    # -----------------------------
    # MENU
    # -----------------------------

    @rumps.clicked("Start Relax")
    def manual_relax(self, _):
        self.start_relax()

    @rumps.clicked("Pause")
    def pause(self, sender):
        if self.timer:
            self.timer.cancel()

        self.state = "PAUSED"
        self.title = "⏸ PAUSED"
        sender.title = "Resume"


if __name__ == "__main__":
    EyeCareApp().run()