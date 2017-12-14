INC=/usr/local/ssl/include/
LIB=/usr/local/ssl/lib/

a:
	g++ -I$(INC) -L$(LIB) -o a A.cpp -lcrypto -ldl
b:
	g++ -I$(INC) -L$(LIB) -o b B.cpp -lcrypto -ldl
km:
	g++ -I$(INC) -L$(LIB) -o km KM.cpp -lcrypto -ldl	