								���������:
								
����������� JassPerformCounter.mix � ����� � �����

								�������������:

��� ������������� �������� 2 ������ ����� ����������� ���������� ����������.

constant native WriteLogFile                 takes string s returns nothing
constant native PerfCounterGetSec            takes integer id returns string
constant native StartPerfCounter             takes integer id returns nothing
constant native StopPerfCounter              takes integer id returns nothing
constant native PerfCounterPrintResult       takes integer id returns nothing

ID - id �������� �� 0 �� 99

������� ����� ��������� ������� � ������� StartPerfCounter( ID ) 
����� ���������� � ������� StopPerfCounter( ID )
����� ����� �������������� PerfCounterPrintResult( ID ) ��� ������ ���������� �� ����� 
��� �� WriteLogFile(PerfCounterGetSec( ID )) ��� ������ � ���-����

StartPerfCounter - ��������� ������� ������������������.

StopPerfCounter - ������������� � ������� ���������.