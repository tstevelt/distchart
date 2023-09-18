#!/bin/sh


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
#	echo "select Sclose from stock where Stype = 'S' and Sclose > 10 and Sclose < 100.0" | mysql -D invest > Prices.csv
#	CleanFile Prices.csv
	distchart -r 25 -in Prices.csv -h
}

AnalyzePE ()
{
#	echo "select Fpettm from stock, fundamental where Sticker = Fticker and  Stype = 'S' and Fpettm > 0 and Fpettm < 40" | mysql -D invest > PE.csv
#	CleanFile PE.csv
	distchart -r 25 -in PE.csv -h
}

AnalyzeFood ()
{
#	echo "select Fid, Fcalorie, Fcarb, Fprotein, Ffat, Fsodium, Ffiber from food" | mysql -D nutrition > Food.csv
#	CleanFile Food.csv
	distchart -r 25 -in Food.csv -h -c 2 7 -d '|'
}

AnalyzeSP500 ()
{
	distchart -r 25 -in sp500_norm.csv
}

AnalyzeBenford ()
{
#	echo "select Sclose from stock where Sclose > 0.0" | mysql -D invest > StockPricesBenford.csv
#	CleanFile StockPricesBenford.csv
#	mv StockPricesBenford.csv StockPricesBenford.csv-
#	cat StockPricesBenford.csv- | textp -f | cut -c1-1 | grep -e '[1-9]' > StockPricesBenford.csv
	distchart -r 9 -in StockPricesBenford.csv
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

rm -f *- script

