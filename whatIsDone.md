# IRC Protocol and Commands

The IRC (Internet Relay Chat) protocol is defined in RFC 1459, and later updated by RFC 2810, RFC 2811, RFC 2812, and RFC 2813. These RFCs collectively describe the IRC protocol and its various functionalities and commands. Here is a summary of the key functionalities and commands defined in the IRC RFC:

## Subject Requirements

- You must be able to connect the reference client to your server without producing any errors.
- You must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using this client.
- All messages from one client on a channel are sent to all other clients of the channel.
- You must have operators and regular users.

</br>
</br>

<span style="color:green">(CD)</span> = Durrently Doing

## User Authentication and Connection

- `PASS`: Set a connection password. ✅
- `NICK`: Set the user's nickname. ✅
- `USER`: Specify the user's username, hostname, servername, and real name. ✅

## Channel Operations

- `JOIN`: Join a channel or a group of channels. ✅
- `INVITE`: Invite a user to a channel ✅
- `PART`: Leave a channel. ✅
- `TOPIC`: Set or get the topic of a channel. ✅
- `NAMES`: Retrieve the list of users in a channel. 
- `LIST`: List all channels or specific channels. ✅

## Messaging

- `PRIVMSG`: Send a private message to a user or a channel. ✅
- `NOTICE`: Send a notice message to a user or a channel. 

## Server Commands

- `PING`: PING triggers when a server sends a client aPING message to see if its still connected
- `PONG`: Respond to a PING message.

## Operator and Administrative Commands

Include any additional operator-specific actions or commands that your server supports here.
- `KICK`: Kick a user from a channel. ✅
.

## User Information and Tracking

- `WHO`: Get information about users on the network.
- `WHOIS`: Get detailed information about a specific user.
- `WHOWAS`: Get information about previously connected users.
