#!/bin/sh 

HaveDatabase=N

CleanFile ()
{
	FILE=$1

	mv $FILE $FILE-
	grep -v '\+' $FILE- > $FILE

	echo "1,\$s/^..//" > script
	echo "1,\$s/|$//" >> script
	sedit $FILE
}

AnalyzePrices ()
{
	if [ "$HaveDatabase" = 'Y' ]
	then
		echo "select Sclose from stock where Stype = 'S' and Sclose > 10 and Sclose < 100.0" | mysql -D invest > Prices.csv
		CleanFile Prices.csv
	fi
	distchart -r 25 -in Prices.csv -h
}

AnalyzePE ()
{
	if [ $HaveDatabase = Y ]
	then
		echo "select Fpettm from stock, fundamental where Sticker = Fticker and  Stype = 'S' and Fpettm > 0 and Fpettm < 40" | mysql -D invest > PE.csv
		CleanFile PE.csv
	fi
	distchart -r 25 -in PE.csv -h
}

AnalyzeFood ()
{
	if [ $HaveDatabase = Y ]
	then
		echo "select Fid, Fcalorie, Fcarb, Fprotein, Ffat, Fsodium, Ffiber from food" | mysql -D nutrition > Food.csv
		CleanFile Food.csv
	fi
	distchart -r 25 -in Food.csv -h -c 2 7 -d '|'
}

AnalyzeSP500 ()
{
	if [ $HaveDatabase = Y ]
	then
		mkdata -simple -o x1.csv -where sp500
		cat x1.csv | grep -v TICKER | sed 's/^.*[A-Z],//' | sed 's/,.,.,.$//' > x2.csv
		Normalize -in x2.csv -out x3.csv -cols 1 75
		textp -w < x3.csv | grep -ve '^.$' > sp500_norm.csv
		less sp500_norm.csv
	fi
	distchart -r 25 -in sp500_norm.csv
}

AnalyzeBenford ()
{
	if [ $HaveDatabase = Y ]
	then
		echo "select Stype, count(*), avg(Sclose) from stock where Sclose > 0 group by  Stype " | mysql -D invest > StockStats.csv

		echo "select Sclose from stock where Sclose > 0.0" | mysql -D invest > StockPricesBenfordAll.csv
		CleanFile StockPricesBenfordAll.csv
		mv StockPricesBenfordAll.csv StockPricesBenfordAll.csv-
		cat StockPricesBenfordAll.csv- | textp -f | cut -c1-1 | grep -e '[1-9]' > StockPricesBenfordAll.csv

		echo "select Sclose from stock where Sclose > 0.0 and Stype = 'S'" | mysql -D invest > StockPricesBenfordStock.csv
		CleanFile StockPricesBenfordStock.csv
		mv StockPricesBenfordStock.csv StockPricesBenfordStock.csv-
		cat StockPricesBenfordStock.csv- | textp -f | cut -c1-1 | grep -e '[1-9]' > StockPricesBenfordStock.csv

		echo "select Sclose from stock where Sclose > 0.0 and Stype = 'A'" | mysql -D invest > StockPricesBenfordADR.csv
		CleanFile StockPricesBenfordADR.csv
		mv StockPricesBenfordADR.csv StockPricesBenfordADR.csv-
		cat StockPricesBenfordADR.csv- | textp -f | cut -c1-1 | grep -e '[1-9]' > StockPricesBenfordADR.csv

		echo "select Sclose from stock where Sclose > 0.0 and Stype = 'E'" | mysql -D invest > StockPricesBenfordETF.csv
		CleanFile StockPricesBenfordETF.csv
		mv StockPricesBenfordETF.csv StockPricesBenfordETF.csv-
		cat StockPricesBenfordETF.csv- | textp -f | cut -c1-1 | grep -e '[1-9]' > StockPricesBenfordETF.csv
	fi

	cat StockStats.csv

	distchart -r 9 -in StockPricesBenfordAll.csv
	distchart -r 9 -in StockPricesBenfordStock.csv
	distchart -r 9 -in StockPricesBenfordADR.csv
	distchart -r 9 -in StockPricesBenfordETF.csv
}

Pause ()
{
	echo "Press [ENTER] to continue"
	read x
}

AnalyzePrices
Pause

AnalyzePE 
Pause

AnalyzeFood
Pause

AnalyzeSP500
Pause

AnalyzeBenford

rm -f *- script x*.csv

