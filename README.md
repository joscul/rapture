# Get started

### Generate SSH key pair. Do this once and keep those keys for some time.
```
ssh-keygen -t ed25519 -C "your_email@example.com"
```

### Checkout code:
Do this once when you want to start editing code on your computer.
```
git clone git@github.com:joscul/rapture.git
```

### Update your codebase:
Do this every time you start a new session.
```
git pull
```

### View status of your local changes:
Do this when you are unsure what changes you have made.
```
git status
git diff
```

### Commit your changes:
Do this after you have coded something awesome. You will be prompted to write a message, just save and quit the file after you have written a good commit message
```
git add .
git commit
git push
```
