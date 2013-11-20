#include <stdlib.h>
#include <libpq-fe.h>

const char dbname[20] = "myDb";
const char user[20] = "mateuszek";
const char pass[20] = "mateuszek";

void execSql(PGconn *connection, char *query){
	PGresult *status;
	system("clear");
	printf("wykonywanie polecenia : \n%s \n\n",query);
	status=PQexec(connection,query);
	printf("Status polecenia: %s \n",PQresStatus(PQresultStatus(status)));
	printf("%s \n",PQresultErrorMessage(status));
    switch(PQresultStatus(status)) {
	  case PGRES_TUPLES_OK:
		{
		  int rec_num, col_num;
		  int cols = PQnfields(status);
		  int records   = PQntuples(status);
		  for(rec_num = 0; rec_num < records; rec_num++) {
			for(col_num = 0; col_num < cols; col_num++)
			  printf(" %s = %s", PQfname(status, col_num),PQgetvalue(status,rec_num,col_num));
			printf("\n\n");
		  }
		}
	  }
	  PQclear(status);
	 
	 
}

void createTable(PGconn *connection){
	execSql(connection,"CREATE TABLE pracownik(pracownik_id serial PRIMARY KEY,imie varchar(30) NOT NULL,nazwisko varchar(50) NOT NULL,pensja money,data_u date,stanowisko varchar(50) NOT NULL,nr integer)");
}
void dropTable(PGconn *connection){
	execSql(connection,"DROP TABLE pracownik");
}
void addRecord(PGconn *connection){
	char imie[45],nazwisko[50],stanowisko[50],data[10],query[512];
	double pensja;
	int nr;
	printf(" wpisz Imie\n"); scanf("%s",imie);	
	printf(" wpisz Nazwisko\n"); scanf("%s",nazwisko);	
	printf(" wpisz Pensje\n"); scanf("%lf",&pensja);	
	printf(" wpisz date urodzenia\n"); scanf("%s",data);	
	printf(" wpisz stanowisko\n"); scanf("%s",stanowisko);
	printf(" wpisz nr pracownika\n"); scanf("%d",&nr);
	printf("%lf",pensja);
    sprintf(query,"INSERT INTO pracownik(imie,nazwisko,pensja,data_u,stanowisko,nr) VALUES(\'%s\',\'%s\',%6.2lf,\'%s\',\'%s\',%d)",imie,nazwisko,pensja,data,stanowisko,nr);
    execSql(connection,query);
}

void editRecord(PGconn *connection){

	char column[50],query[512];
	int id;
	showAllRecords(connection);
	printf("nazwa edytowanej kolumny:\n (imie,nazwisko,pensja,data_u,stanowisko,nr)");
	scanf("%s",column);
	printf("id edytowanego rekordu:\n (imie,nazwisko,pensja,data_u,stanowisko,nr)");
	scanf("%d",&id);
	printf("\nPodaj nowa wartosc dla kolumny %s\n",column);
	if (strcmp(column,"imie")==0 || strcmp(column,"nazwisko")==0 || strcmp(column,"stanowisko")==0 || strcmp(column,"data")==0){
		char val[50];	
		scanf("%s",val);
		sprintf(query,"UPDATE pracownik SET %s = \'%s\' WHERE pracownik_id = %d ",column,val,id);
		execSql(connection,query);
	
	} 
	else{
		if(strcmp(column,"pensja")==0){
			double val;
			scanf("%lf",&val);
			sprintf(query,"UPDATE pracownik SET %s = %6.2lf WHERE pracownik_id = %d ",column,val,id);
			execSql(connection,query);
		}
		else{
			if(strcmp(column,"nr")==0){
				int val;
				scanf("%d",&val);
				sprintf(query,"UPDATE pracownik SET %s = %d WHERE pracownik_id = %d ",column,val,id);
				execSql(connection,query);
			}else{
			printf("\nbrak kolumny o nazwie %s\n",column);
			}
		}
	}

	}
void deleteRecord(PGconn *connection){
	char query[512];
	int id;
	printf("Wpisz id rekordu ktory usunac");
	scanf("%d",&id);
	sprintf(query,"DELETE FROM pracownik WHERE pracownik_id=%d",id);
	execSql(connection,query);
}
void searchByBirthDate(PGconn *connection){
	char date_f[10],date_t[10],query[512];
	printf("\tZnajdz pracownika po dacie\n\n");
	printf("data od: ");
	scanf("%s",date_f);
	printf("\ndata do: ");
	scanf("%s",date_t);
	sprintf(query,"SELECT * FROM pracownik WHERE data_u between \'%s\' and \'%s\' ORDER BY pracownik_id",date_f,date_t);
	execSql(connection,query);
	


}
void searchByPosAndSalary(PGconn *connection){
	char position[50],salary[35],query[512];
	printf("\tZnajdz pracownika po Stanowisku i pensji\n\n");
	printf("Stanowisko: ");
	scanf("%s",position);
	printf("\nNazwisko: ");
	scanf("%s",salary);
	sprintf(query,"SELECT * FROM pracownik WHERE stanowisko = \'%s\' and nazwisko = \'%s\' ORDER BY pracownik_id",position,salary);
	execSql(connection,query);
	


}

void showAllRecords(PGconn *connection){
execSql(connection,"SELECT * FROM pracownik ORDER BY pracownik_id");
}
int main()
{
  // próba po³±czenia
  PGconn *myconnection = PQconnectdb("host=localhost port=5432 dbname=myDb user=mateuszek password=mateuszek");
  //PGconn *myconnection = PQconnectdb("");
  // sprawdzamy status po³±czenia
  if(PQstatus(myconnection) == CONNECTION_OK) {
		printf("connection made\n");
		// informacje o po³±czeniu
		printf("PGDBNAME   = %s\n",PQdb(myconnection));
		printf("PGUSER     = %s\n",PQuser(myconnection));
		//printf("PGPASSWORD = %s\n",PQpass(myconnection));
		printf("PGPASSWORD = ********\n");
		printf("PGHOST     = %s\n",PQhost(myconnection));
		printf("PGPORT     = %s\n",PQport(myconnection));
		printf("OPTIONS    = %s\n",PQoptions(myconnection));
  }
  else{
	  printf("connection failed: %s\n", PQerrorMessage(myconnection));
	  // w razie utraty po³±czenia wywo³anie 
	  // PQreset(myconnection);
	  // zamyka op³±czenie i nawi±zuje je raz jeszcze  
	  // z dotychczasowymi parametrami
	  PQfinish(myconnection);
	  return EXIT_SUCCESS;
	}
  int k=1;
  int choice;
  while (k==1){
	  printf("wpisz: \n");
	  printf("\t '0': aby zakonczyc\n");
	  printf("\t '1' : aby utworzyc tabele\n");
	  printf("\t '2' : aby usunac tabele\n");
	  printf("\t '3' : aby dodac rekord\n");
	  printf("\t '4' : aby edytowac rekord\n");
	  printf("\t '6' : aby wyswietlic wszystkie rekordy\n");
	  printf("\t '7' : wyszukaj pracownika po dacie urodzenia\n");
	  printf("\t '8' : wyszukaj pracownika po stanowisku i nazwisku\n");
	  scanf("\t%d",&choice);
	  switch (choice){
		  case 1 :  system("clear");
					createTable(myconnection);
					break;
		  case 2 :  system("clear");
					dropTable(myconnection);
					break;
		  case 3 :  system("clear");
					addRecord(myconnection);
					break;
		  case 4 :	system("clear");
					editRecord(myconnection);
					break;		
		  case 5 :	system("clear");
					deleteRecord(myconnection);
					break;				
		  case 6 :  system("clear");
					showAllRecords(myconnection);
					break;					
					
		  case 7 :  system("clear");
					searchByBirthDate(myconnection);
					break;			
		  case 8 :  system("clear");
					searchByPosAndSalary(myconnection);
					break;		
		  default:  system("clear");
				    printf("Nieodpowiedni wybor ('%d').\n\n",choice);
	  }
   
  }
}

