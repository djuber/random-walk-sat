#!/bin/sh


PROGRAM=walksat.exe
PROJECT=random-walk-sat

# build program: move to parent
make windows
cp $PROGRAM ..
make clean

# create zip archive of current directory
cd ../..
rm -f CS408-Project2-Uber.zip
zip -r CS408-Project2-Uber.zip $PROJECT -i $PROJECT/README  -i $PROJECT/src/* -i $PROJECT/$PROGRAM


rm -f $PROJECT/$PROGRAM

