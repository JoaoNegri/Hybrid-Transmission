# Hybrid Transmission Project

This repository contains an implementation of the **Hybrid Transmission (HT)** protocol, which increases the reliability of wireless communication by combining message replication and erasure coding.

The project is composed of two main components:

- **Esp-firmware (ESP32 + LoRaWAN)**: responsible for handling transmission logic and communicating with The Things Network.
- **Server**: handles server-side functionalities and can be used for monitoring or post-processing.

> 📚 The protocol implementation is based on the article:
> > J. M. d. S. Sant’Ana, A. Hoeller, R. D. Souza, S. Montejo-Sánchez, H. Alves and M. d. Noronha-Neto,  
> > "*Hybrid Coded Replication in LoRa Networks,*"  
> > IEEE Transactions on Industrial Informatics, vol. 16, no. 8, pp. 5577–5585, 2020.  
> > DOI: [10.1109/TII.2020.2966120](https://doi.org/10.1109/TII.2020.2966120)

## 📁 Project Structure

```plaintext
Hybrid-Transmission/
├── Esp32-firmware/       # C++ firmware using PlatformIO for ESP32
├── server/               # Lightweight Python server
└── README.md             # This file (project overview)
