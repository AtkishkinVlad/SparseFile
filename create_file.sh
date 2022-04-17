echo -ne '\001'
for ((i=1; i<10000;i++ ))
do
  echo -ne '\000'
done
echo -ne '\001' 
for ((i=10000; i<4*1024*1024;i++ ))
do
  echo -ne '\000'
done
echo -ne '\001'
