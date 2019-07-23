#include "tower.h"
#include "util.h"

int DebugPrintf(char* pszFmt, ... ) {

  int cargs = 0;
  char* pszTmp;

  for (pszTmp = pszFmt; *pszTmp; pszTmp++)  
    if (*pszTmp == '%')  
      cargs++; 

  va_list argv;
  va_start(argv, cargs);

  pszTmp = pszFmt;
  while (*pszTmp)
    {
      if (*pszTmp == '%')
        {
	  pszTmp++;
	  switch (*pszTmp)
            {
	    case 'd':   
	      Serial.print(va_arg(argv, int));    
	      break;

	    case 'x':   
	      Serial.print(va_arg(argv, int), HEX);    
	      break;

	    case 'l':   
	      Serial.print(va_arg(argv, long));
	      break;

	    case 'u':
	      Serial.print(va_arg(argv, unsigned long));
	      break;

	    case 'f':
	      Serial.print(va_arg(argv, double));
	      break;

	    case 'F':
	      Serial.print(va_arg(argv, double), 8);
	      break;

	    case 'c':
	      Serial.print((char) va_arg(argv, int));
	      break;

	    case 's':
	      Serial.print(va_arg(argv, char*));
	      break;

	    case '%':
	      Serial.print('%');
	      break;

	    default:
	      break;
            }
        }
      else if (*pszTmp == '\n')
        {
	  Serial.println();
        }
      else
        {
	  Serial.print(*pszTmp);
        }

      pszTmp++;
    }
}
