#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}



  char *itoa(unsigned int num, int base)                           // unsigned itoa (convertor in to string cu conversie de baza)
{                                                                 //  lucreaza cu numere naturale
	static char Representation[]= "0123456789abcdef";               
	static char buffer[11];                                      // UINT_MAX = 4294967295 are lungimea zece, deci bufferul va 
	char *ptr;                                                   // avea nevoie de maxim 11 octeti
	
	ptr = &buffer[11]; 
	*ptr = '\0'; 
	
	do 
	{ 
		*--ptr = Representation[num%base];                      // convertire la baza ceruta prin impartire cu pastrarea restului
		num /= base; 
	}while(num != 0); 
	
	return(ptr);                                            
}


int iocla_printf(const char *format, ...) {
	char *formatted = (char *) malloc(strlen(format) + 1);         // stringul in care va fi pus outputul formatat 
	va_list args;
	int indexFormatted = 0;                                       // index pt sirul formatat
	int nr;
	unsigned int unsignedNr;
	int totalSize  = strlen(format) + 1;                         // marimea totala a sirului. Folosita si actualizata
	char buffer[100];                                            // la realocarea memoriei  

    va_start(args, format);

	for (int i = 0; i < strlen(format); i++) {                     // daca nu apare caracterul
		if (format[i] != '%') {                                   //  indicator de argument '%' se scriu caracterele asa cum sunt 
			formatted[indexFormatted++] = format[i];             
		}  else {                                                 // verifica daca caracterul '%' gasit este
			switch(format[i + 1]) {                              //  un indicator de argument sau un simplu '%' 
 				case 'd':                                        
				    nr = va_arg(args, int);                       
				    if (nr < 0) {                               // daca numarul este negativ adauga '-' si converteste modulul la string
				    	formatted[indexFormatted++] = '-';
				    	totalSize += 1;
				    	nr *= -1;
				    }
				    strcpy(buffer, itoa(nr, 10));
				    if (strlen(buffer) > 2) {                  //daca string-ul este mai mare decat 2(|%d|) extinde marimea sirului
				    	formatted = (char *) realloc(formatted, totalSize + strlen(buffer) - 2);  // si retine noua marime
				    	totalSize += strlen(buffer) - 2;
				    }
				    memcpy(formatted + indexFormatted, buffer, strlen(buffer) + 1);
				    indexFormatted += strlen(buffer);
				    i++;
				    break;

				case 'u':                                     // idem int fara cazul numarului negativ
					unsignedNr = va_arg(args, unsigned int);
					strcpy(buffer, itoa(unsignedNr, 10));
					if (strlen(buffer) > 2) {
				    	formatted = (char *) realloc(formatted, totalSize + strlen(buffer) - 2);
				    	totalSize += strlen(buffer) - 2;
				    }
					memcpy(formatted + indexFormatted, buffer, strlen(buffer) + 1);
					indexFormatted += strlen(buffer);
					i++;
					break;
				case 'x':                                  // idem uint dar alta baza
					unsignedNr = va_arg(args, unsigned int);
					strcpy(buffer, itoa(unsignedNr, 16));
					if (strlen(buffer) > 2) {
				    	formatted = (char *) realloc(formatted, totalSize + strlen(buffer) - 2);
				    	totalSize += strlen(buffer) - 2;
				    }
					memcpy(formatted + indexFormatted, buffer, strlen(buffer) + 1);
					indexFormatted += strlen(buffer);
					i++;
					break;
				case 'c':                         //este suficienta citirea valorii ascii intr-un int si 
					nr = va_arg(args, int);       // adaugarea ei in sir
					formatted[indexFormatted++] = nr;
					i++;
					break;

				case 's':
					strcpy(buffer, va_arg(args, char *));   // copiaza argumentul in buffer apoi din buffer in sir
					if (strlen(buffer) > 2) {
				    	formatted = (char *) realloc(formatted, totalSize + strlen(buffer) - 2);
				    	totalSize += strlen(buffer) - 2;
				    }
					memcpy(formatted + indexFormatted, buffer, strlen(buffer) + 1);
					indexFormatted += strlen(buffer);
					i++;
					break;	
				default:                                      // daca '%' nu este indicator de argument
					formatted[indexFormatted++] = format[i];  // il punem asa cum e in sir
					i++; 		         

			}
		}
	}
	formatted[indexFormatted] = 0;   
	

	va_end(args);

	return write_stdout(formatted, indexFormatted);
}
