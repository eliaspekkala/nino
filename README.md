# nino
A small terminal-based text editor written in C.

I made this a while ago following [snaptoken's Build Your Own Text Editor tutorial](https://viewsourcecode.org/snaptoken/kilo/)

Also, see the original text editor the tutorial based on: [kilo](https://github.com/antirez/kilo)

## Features
- Basic syntax highlight
- Find
- Line numbers
- Auto indent
- Auto bracket completion
- Selected text
- Go to line number
- Configs

## Configs
Set config using `ctrl+p` or create the config file `~/.ninorc` 

Example:
```
tabsize 8
color status.fg 0078d7
```
- `tabsize` [size]
- `color` [target] [color]
    - `status.fg`
    - `status.bg`
    - `hl.normal`
    - `hl.comment`
    - `hl.keyword1`
    - `hl.keyword2`
    - `hl.keyword3`
    - `hl.string`
    - `hl.number`
    - `hl.match`

## Install from source
```
git clone https://github.com/evanlin96069/sdp-c.git
cd nino
./compile.sh
sudo ./install.sh
```

