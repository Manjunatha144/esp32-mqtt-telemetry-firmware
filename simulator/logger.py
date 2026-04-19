import csv
import time
import json
import paho.mqtt.client as mqtt

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC_TLM = "edgepulse/manjunatha144/device_001/telemetry"
OUT_CSV = "simulator/telemetry.csv"

def on_connect(client, userdata, flags, rc, properties=None):
    print("connected rc=", rc)
    client.subscribe(TOPIC_TLM)

def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload.decode("utf-8"))
        row = [
            int(time.time() * 1000),
            data.get("device_id", ""),
            data.get("seq", ""),
            data.get("uptime_ms", ""),
            data.get("temp", ""),
        ]
        userdata["writer"].writerow(row)
        userdata["file"].flush()
        print("logged seq=", data.get("seq"))
    except Exception as e:
        print("log error:", e)

def main():
    f = open(OUT_CSV, "a", newline="")
    w = csv.writer(f)
    if f.tell() == 0:
        w.writerow(["ts_ms", "device_id", "seq", "uptime_ms", "temp"])
        f.flush()

    client = mqtt.Client(userdata={"writer": w, "file": f})
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(BROKER, PORT, 60)
    client.loop_forever()

if __name__ == "__main__":
    main()