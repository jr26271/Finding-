#!/usr/bin/env bash

rm -rf build
mkdir build
javac -d ./build Main.java ../Verifier.java
cd build
java -cp ../../soot-4.2.1-jar-with-dependencies.jar soot.Main -cp . -pp -f jimple Main -write-local-annotations -p jb use-original-names:true -keep-line-number -print-tags-in-output
cd ..
cp ./build/sootOutput/Main.jimple .