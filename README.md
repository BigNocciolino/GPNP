# NEMA READER

This is a little utility to parse the data coming from a gps module.

**This project is in a really alpha state, it may contains bug**

At the moment the program can oly parse the `$GPRMC` sentence and create a output gpx file with timestamps latitude and longitude

## Dependencies

- [MXML](https://www.msweet.org/mxml/)

## Build

``` bash
    git clone https://github.com/BigNocciolino/GPS_parser.git
    cd GPS_parser/
    make
```

## Run

At the moment the program accepts 2 parameters, [1]= the path to the .txt file with all the sentences, [2]= the path to the output .gpx file

This is only an example, make sure to use your data
```bash
    ./build/decoder data/NEMA_short.txt out/test.gpx
```


## Future for this project 

- Expand its capabilites to parse all non-proprietary `GP` sentences
- Make it much easiner to use 
- Create a UI (or interactive shell)