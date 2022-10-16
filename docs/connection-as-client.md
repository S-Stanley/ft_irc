# Connexions as clients

### Step 1

First you must install telnet

```bash
brew install telnet
```

Then

```bash
telnet [HOST] [PORT]
```

Example:

```bash
telnet localhost 6667
```

IRC servers default port is `6667`

### Step 2

Then send the username and nickname using the `USER` and `NICK` commands.

```bash
USER demo_username
```

```bash
NICK demo_nickname
```