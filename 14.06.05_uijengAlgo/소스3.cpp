#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <tchar.h>
#include <atlstr.h>

int Folder_access(CString p);
int Make_file(CString path);

int main()
{
   CString root(_T("C:\\virustest\\"));
   Folder_access(root);
   return 0;
}

int Folder_access(CString p)
{
   WIN32_FIND_DATA finddata;
   HANDLE hfind;
   CString wild(_T("\\*"));

   _wsetlocale(LC_ALL, _T("korean"));

   if (PathIsDirectoryEmpty(p))
   {
      Make_file(p+"\\");
      printf("empty\n");
      return 0;
   }
   else
   {
      hfind = FindFirstFile(p + wild, &finddata);

      do
      {
         if ((finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
         {
            CString d(finddata.cFileName);
            if (d != _T(".") && d != _T(".."))
            {

               Make_file(p + '\\');
               Folder_access(p + '\\' + d);
               _tprintf(_T("%s\n"), finddata.cFileName);
            }
         }
      } while (FindNextFile(hfind, &finddata));

   }

   FindClose(hfind);

   return 0;
}

int Make_file(CString path)
{
   FILE *f;

   CString name(_T("hack.txt"));
   path += name;
   const char *str;
   str = (CStringA)path;

   fopen_s(&f, str, "w");
   fprintf(f, "hello!");
   fclose(f);

   return 0;
}