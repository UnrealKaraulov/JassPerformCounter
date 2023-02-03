								”становка:
								
ѕереместить JassPerformCounter.mix в папку с игрой

								»спользование:

ƒл€ использовани€ добавить 2 строки перед объ€влением глобальных переменных.

constant native WriteLogFile                 takes string s returns nothing
constant native PerfCounterGetSec            takes integer id returns string
constant native StartPerfCounter             takes integer id returns nothing
constant native StopPerfCounter              takes integer id returns nothing
constant native PerfCounterPrintResult       takes integer id returns nothing

ID - id счетчика от 0 до 99

Сначала нужно запустить счетчик с помощью StartPerfCounter( ID ) 
затем остановить с помощью StopPerfCounter( ID )
Потом можно использововать PerfCounterPrintResult( ID ) для вывода результата на экран 
или же WriteLogFile(PerfCounterGetSec( ID )) для вывода в лог-файл

StartPerfCounter - запускает счетчик производительности.

StopPerfCounter - останавливает и выводит результат.