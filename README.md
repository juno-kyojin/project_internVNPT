# CAPWAP Implementation without DTLS

Welcome to the CAPWAP (Control And Provisioning of Wireless Access Points) implementation project. This project includes an **Agent** and a **Server** that communicate using the CAPWAP protocol over UDP without utilizing DTLS (Datagram Transport Layer Security).

## Table of Contents

1. [Overview](#overview)
2. [Prerequisites](#prerequisites)
3. [Project Structure](#project-structure)
4. [Compilation Instructions](#compilation-instructions)
5. [Running the Programs](#running-the-programs)
6. [Interpreting Hexadecimal Data](#interpreting-hexadecimal-data)
    - [1. CAPWAP Message Structure](#1-capwap-message-structure)
    - [2. Discovery Request from Agent to Server](#2-discovery-request-from-agent-to-server)
    - [3. Discovery Response from Server to Agent](#3-discovery-response-from-server-to-agent)
    - [4. Example Analysis](#4-example-analysis)
    - [5. Sample Parsing Script](#5-sample-parsing-script)
7. [Troubleshooting](#troubleshooting)
8. [Additional Information](#additional-information)
9. [License](#license)

---

## Overview

CAPWAP is a protocol used to manage and control wireless access points (APs). In this implementation:

- **Agent**: Represents the wireless access point.
- **Server**: Represents the wireless LAN controller.

This project facilitates the discovery process where the Agent sends a Discovery Request to the Server, and the Server responds with a Discovery Response containing configuration details.

**Note**: This implementation excludes DTLS, meaning the communication between Agent and Server is unencrypted. Ensure that the environment where this is deployed is secure.

---

## Prerequisites

- **Operating System**: Linux-based system.
- **Compiler**: GCC (GNU Compiler Collection).
- **Libraries**:
  - OpenSSL (for random number generation using `libcrypto`).
- **Tools**:
  - `make` (optional, if Makefile is used).
  - `Wireshark` (optional, for network traffic analysis).

### Installing Dependencies

On Debian/Ubuntu systems, install the necessary libraries using:

```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev
