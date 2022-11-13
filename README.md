# NMEA PARSER

This is a little utility to parse the data coming from a gps module.

**This project is in a really alpha state, it may contains bug**

## Dependencies

- [MXML](https://www.msweet.org/mxml/)
- [LIBNMEA](https://github.com/BigNocciolino/libnmea)

## Build

``` bash
    git clone https://github.com/BigNocciolino/GPS_parser.git
    cd GPS_parser/
    make
    sudo make install
```

## Usage

This is only an example, make sure to use your data
#### Using single files
```bash
    gpnp -i data/NEMA_short.txt -o out/test.gpx
```
#### Using folders
The program will take every file in the input folder, parse and output it in the output folder.
```bash
  gpnp -i data/ -o out/
```
This will use the same folder as output
```bash
  gpnp -i data/
```


## Future for this project 

- Expand its capabilites to parse all non-proprietary `GP` sentences
- Create a UI (or interactive shell)