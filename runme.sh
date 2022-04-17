source create_file.sh > fileA.dat
make 
./sparse fileA.dat fileB.dat
gzip -k fileA.dat
gzip -k fileB.dat
gzip -cd fileB.dat.gz | ./sparse fileC.dat
./sparse fileA.dat fileD.dat -b 100
source sizes.sh > result.txt
