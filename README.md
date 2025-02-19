# ft_traceroute

> This repository contains 'ft_traceroute' project that is part of the École 42 curriculum.

## Overview
**ft_traceroute** is a 42 École outer core project focused on networking and system-level programming. The goal is to implement a program in C that replicates the functionality of the **traceroute** command, allowing users to trace the route that packets take to reach a destination host across a network.

This project explores **Layer 3 (Network Layer) of the OSI model**, providing insights into core networking concepts such as: **ICMP (Internet Control Message Protocol)**, **UDP (User Datagram Protocol)**, **TTL (Time To Live)**, **packet routing**, and **network latency measurement**. By implementing ft_traceroute, students gain a deeper understanding of how data travels through networks, how routers process packets, and how to diagnose connectivity issues by analyzing hops and response times.

## Key Features of My Implementation
- **Dual-socket approach:**
  - **UDP socket** for sending requests to the target host. Packets are sent to port **33434**, incrementing by 1 for each hop (to avoid conflicts with existing services).
  - **ICMP socket** for receiving responses.

- **Packet transmission and reception:**
  - Sends **3 UDP** packets per hop.
  - Uses `select()` to wait for ICMP responses with a **timeout margin of 150000 microseconds** between each reception.

- **Handled response cases:**
  - **ICMP "Time-to-live exceeded"** – Indicates the packet reached a router, but TTL expired.
  - **ICMP "Destination port unreachable"** – The packet reached the target host, but the port is closed (indicates success, program terminates).
  - **Timeout scenario** – If no response is received within 150000 microseconds, outputs * to indicate packet loss or an unresponsive hop.

## Subject
All the details of this project, including the specific requirements and implementation guidelines, are thoroughly described in the `en.subject.pdf` file.

