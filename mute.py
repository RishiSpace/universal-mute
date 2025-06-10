import subprocess
from plyer import notification

def main():
    # Launch mute.exe. Make sure mute.exe is in the same directory or provide the full path.
    process = subprocess.Popen(["mute.exe"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    
    # Continuously read output from mute.exe
    while True:
        line = process.stdout.readline()
        if not line:
            break
        line = line.strip()
        if line.startswith("Microphone"):
            notification.notify(
                title="Microphone Status",
                message=line,
                timeout=5  # seconds
            )

if __name__ == "__main__":
    main()