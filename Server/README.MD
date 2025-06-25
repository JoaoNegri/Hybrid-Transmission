# Hybrid Transmission — Python Server

This folder contains a simple Python server responsible for:

- Receiving messages transmitted by the ESP32 node (via LoRaWAN/TTN/MQTT).
- Decoding the Hybrid Transmission (HT) protocol packets.
- Persisting messages on disk for later analysis.

The server expects incoming messages to follow the format defined in the protocol implementation.

## 🛠️ Requirements

Install dependencies using:

```bash
pip install -r requirements.txt
