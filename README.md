# FT_IRC

Welcome to FT_IRC! This project focuses on creating a simple IRC (Internet Relay Chat) server and client using sockets and the TCP/IP protocol. By implementing this project, you'll gain a deeper understanding of network programming, client-server architecture, and communication protocols.

## Table of Contents

1. [Introduction](#introduction)
2. [Usage](#usage)
3. [Features](#features)
4. [Building](#building)
5. [Contributing](#contributing)
6. [Credits](#credits)

## Introduction

FT_IRC is a lightweight IRC server and client implementation designed to facilitate real-time text communication over a network. It provides a platform for users to join chat rooms, exchange messages, and interact with other users in a multi-user environment.

## Usage

To use FT_IRC, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/yourusername/ft_irc.git
    ```

2. Build the server and client executables:

    ```bash
    make
    ```

3. Start the server:

    ```bash
    ./server <port>
    ```

4. Start the client and connect to the server:

    ```bash
    ./client <server_address> <port> <username>
    ```

5. Start chatting with other users!

## Features

FT_IRC supports the following features:

- Creation and joining of chat rooms
- Sending and receiving messages in real-time
- User authentication and identification
- Private messaging between users
- Kick and ban functionality for moderators
- Basic server administration commands

## Building

To build FT_IRC, simply run:

```bash
make
```

## Contributing
Contributions are welcome! If you find any bugs or have suggestions for improvement, please open an issue or submit a pull request on GitHub.

## Credits
This project was created by adrgonza for the ft_irc project at 42Madrid.
