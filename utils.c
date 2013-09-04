

#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "fat_defs.h"
#include "dir_entry.h"

/* 'str' is the string you want to remove characters 'c' from */
char *remove_all_chars(const char* str, char c) {
	char *copy = (char *)malloc(strlen(str));
	strcpy(copy,str);
    	char *pr = copy, *pw = copy;
    	while (*pr) {
        	*pw = *pr++;
        	pw += (*pw != c);
		}
    	*pw = '\0';
	
	return copy;
}

/* PRE: str must be either NULL or a pointer to a 
 * (possibly empty) null-terminated string. */
void strrev(char *str) {
  char temp, *end_ptr;

  /* If str is NULL or empty, do nothing */
  if( str == NULL || !(*str) )
    return;

  end_ptr = str + strlen(str) - 1;

  /* Swap the chars */
  while( end_ptr > str ) {
    temp = *str;
    *str = *end_ptr;
    *end_ptr = temp;
    str++;
    end_ptr--;
  }
}

/* 'replace_chars' contains characters that you want to replace with character 'replace_with' in string str */
void replace_all_chars(char **str, const char* replace_chars, char replace_with) {
	char *c;
	
	c = *str;
   
	while (*c)
	{
	   if (strchr(replace_chars, *c))
	   {
		  *c = replace_with;
	   }

	   c++;
	}
}

/* Returns 1 if a lowercase character is found in 'str', reurns 0 otherwise */
int contains_lowercase(const char *str)
{
	char *c = str;
	
	while (*c)
    {
		if(islower(*c))
		{
			return 1;
		}
		c++;
    }

	return 0;
}

/* http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way */
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

/* http://stackoverflow.com/questions/1071542/in-c-check-if-a-char-exists-in-a-char-array */
int correct_filename(const char* str)
{
   const char *invalid_characters = "\\?:*\"><|";
   char *c = str;
   
   /* Make sure the string is long and short enough */
   if(strlen(str) > 255 || strlen(str) <= 0)
   {
       printf("Invalid filename, strlen: %d\n", strlen(str));
       errno = ENAMETOOLONG;
       return -1;
   }
   
   while (*c)
   {
       if (strchr(invalid_characters, *c))
       {
          printf("Invalid filename, char found: %c\n", *c);
          errno = ENAMETOOLONG;
          return -1;
       }

       c++;
   }

   return 0;
}

char *generate_file_path()
{
	return NULL;
}

char *generate_short_filename(node_entry_t *curdir, char * fn, unsigned char attr, int *lfn)
{
	int diff = 1;
	
	char *temp1 = NULL;
	char *temp2 = NULL;
	
	char *copy = NULL;
	char *integer_string = malloc(7); 
	
	char *fn_temp;
	char *filename = malloc(11); // File length max is 8 chars
	char *ext = malloc(4);      // Extension length max is 3 chars
	char *fn_final;
	
	// 1. Remove all spaces. For example "My File" becomes "MyFile".
	copy = remove_all_chars(fn, ' ');
	
	//printf("Remove Spaces: %s\n", copy);
	
	// 3. Translate all illegal 8.3 characters( : + , ; = [ ] ) into "_" (underscore). For example, "The[First]Folder" becomes "The_First_Folder".
	replace_all_chars(&copy, ":+,;=[]", '_');
	
	//printf("Translate illegal Characters: %s\n", copy);
	
	
	
	
	
	
	

	/* 2. Initial periods, trailing periods, and extra periods prior to the last embedded period are removed.
	For example ".logon" becomes "logon", "junk.c.o" becomes "junkc.o", and "main." becomes "main". */

	temp1 = malloc(strlen(fn)+1);
	
	strncpy(filename, strtok(copy, "."), 10); // Copy Filename
	filename[10] = '\0';
	
	strncpy(ext, strtok(NULL, "."), 3);      // Copy Extension
	ext[3] = '\0';
	
	temp2 = strtok(NULL, ".");
	
	while(temp2 != NULL)
	{
		memset(temp1, 0, strlen(fn)+1);
		strcat(temp1, filename);
		strcat(temp1, ext);
		
		strncpy(filename, temp1, 10); // Copy Filename
		filename[10] = '\0';
		
		strncpy(ext, temp2, 3);
		ext[3] = '\0';
		
		temp2 = strtok(NULL, ".");
	}
	
	//printf("Before Padding Filename: %s Extension: %s \n", filename, ext);
	
	// Pad with spaces if need be
	while(strlen(filename) < 8) // Append spaces to fill end
		strcat(filename, " ");
		
	while(strlen(ext) < 3)
		strcat(ext, " ");
	
	
	//printf("After Padding Filename: %s Extension: %s \n", filename, ext);

/*
4. If the name does not contain an extension then truncate it to 6 characters. If the names does contain
an extension, then truncate the first part to 6 characters and the extension to 3 characters. For
example, "I Am A Dingbat" becomes "IAmADi" and not "IAmADing.bat", and "Super Duper
Editor.Exe" becomes "SuperD.Exe".*/
/*
	// Truncate the filename to 6 chars(length)
	if(strlen(filename) > 6)
		filename[6] = '\0';

	if(strlen(ext) > 3)
		ext[3] = '\0';  //truncate it to 3 characters (ext)
	
	printf("Truncate - Filename: %s Extension: %s \n", filename, ext);
	*/

/*
5. If the name does not contain an extension then a "-1" is appended to the name. If the name does
contain an extension, then a "-1" is appended to the first part of the name, prior to the extension. For
example, "MyFile" becomes "MyFile-1, and "Junk.bat" becomes "Junk-1.bat". This numeric value is
always added to help reduce the conflicts in the 8.3 name space for automatic generated names. 
*/

	if(strlen(filename) > 8)
	{
		// Concate the filename
		memset(temp1, 0, strlen(fn)+1);
		strcat(temp1, filename);
		
		// Append the Value
		memset(integer_string, 0, 7);
		sprintf(integer_string, "~%d", diff++); // Increment diff here for maybe future use
		strcat(temp1, integer_string);
		
		// Append the period
		strcat(temp1, ".");
		
		// Append the ext
		strcat(temp1, ext);
		
		fn_temp = temp1; // Contains the filename and ext
		
		//printf("Getting closer - Filename: %s\n", fn_temp);
		
		*lfn = 1; // This needs a long file name entry
	}
	else
	{
		// Append the filename
		memset(temp1, 0, strlen(fn)+1);
		strcat(temp1, filename);
		
		if(attr == DIRECTORY && ext[0] != ' ')
		{
			// Append the Value
			memset(integer_string, 0, 7);
			sprintf(integer_string, "~%d", diff++); // Increment diff here for maybe future use
			strcat(temp1, integer_string);
		}
		
		// Append the period
		strcat(temp1, ".");
		
		// Append the ext
		strcat(temp1, ext);
		
		fn_temp = temp1; // Contains the filename and ext
		
		//printf("Getting closer - Filename: %s\n", fn_temp);
	}

/*
6. This step is optional dependent on the name colliding with an existing file. To resolve collisions
the decimal number, that was set to 1, is incremented until the name is unique. The number of
characters needed to store the number will grow as necessary from 1 digit to 2 digits and so on. If the
length of the basis (ignoring the extension) plus the dash and number exceeds 8 characters then the
length of the basis is shortened until the new name fits in 8 characters. For example, if "FILENA-
1.EXE" conflicts the next names tried are "FILENA-2.EXE", "FILENA-3.EXE", ..., "FILEN-
10.EXE", "FILEN-11.EXE", etc.
*/

	while(isChildof(curdir, fn_temp) != NULL) // Should not have the same name of any other file/folder in this current directory
	{
		if(diff > 99999)
		{
			printf("Too many entries(Short Entry Name) with the same name(beginning) :p.\n");
			return NULL;
		}
		
		// Rebuild the file name
		memset(temp1, 0, strlen(fn)+1);
		strcat(temp1, filename); // Cat six chars intro temp1
		
		memset(integer_string, 0, 7);
		sprintf(integer_string, "~%d", diff++); // Increment diff here for maybe future use
		
		memcpy(temp1 + (8 - strlen(integer_string)), integer_string, strlen(integer_string));
		
		temp1[8] = '\0';
		
		// Append the period
		strcat(temp1, ".");
		
		// Append the ext
		strcat(temp1, ext);
		
		fn_temp = temp1; // Contains the filename and ext	
		
		//printf("Do over - Filename: %s\n", fn_temp);
	}
	
	// Short Entry Name is unique. Now to copy it to its final string so it can fit nice and snug.
	fn_final = malloc(strlen(fn_temp)+1);
	memset(fn_final, 0, strlen(fn_temp)+1);
	strncpy(fn_final, fn_temp, strlen(fn_temp));
	fn_final[strlen(fn_temp)] = '\0';
	
	if(contains_lowercase(fn_final))
	{
		int i;
		*lfn = 1; // This needs a long file name entry
		
		for(i = 0; i < strlen(fn_final); i++)
			fn_final[i] = toupper(fn_final[i]);
	}
	
	//printf("Final Version - Filename: %s\n", fn_final);

	// Free memory
	free(temp1);
	free(copy);
	
	free(filename);
	free(ext);
	free(integer_string);
	
	return fn_final;
}

fat_lfn_entry_t *generate_long_filename_entry(char * fn, unsigned char checksum, unsigned char order)
{
	char *filename = malloc(13); // 
	fat_lfn_entry_t *lfn_entry = malloc(sizeof(fat_lfn_entry_t));
	
	strncpy(filename,fn, 13);
	
	while(strlen(filename) < 13)  // Pad with spaces if need be
		strcat(filename, " "); 

	lfn_entry->Order = order;
	lfn_entry->Attr = 0x0F ; // Specifying it is a long name entry
	lfn_entry->Checksum = checksum;
	lfn_entry->Cluster = 0;
	strncpy(lfn_entry->FNPart1, filename, 5); // Copy First 5
	lfn_entry->FNPart1[5] = '\0';
	strncpy(lfn_entry->FNPart2, filename+5, 6); // Next 6
	lfn_entry->FNPart2[6] = '\0';
	strncpy(lfn_entry->FNPart3, filename+11, 2); // Last 2
	lfn_entry->FNPart3[2] = '\0';
	
	printf("Generate Long Filename Entry -- Part1: %s, Part2: %s, Part3: %s \n", lfn_entry->FNPart1, lfn_entry->FNPart2, lfn_entry->FNPart3);
	
	free(filename);
	
	return lfn_entry;
}

unsigned char generate_checksum(char * short_filename)
{
	/* CheckSum
	1 	   Take the ASCII value of the first character. This is your first sum.
	2 	   Rotate all the bits of the sum rightward by one bit.
	3 	   Add the ASCII value of the next character with the value from above. This is your next sum.
	4 	   Repeat steps 2 through 3 until you are all through with the 11 characters in the 8.3 filename. 
	*/
	int i;
	char *name;
	unsigned char sum;
	
	name = remove_all_chars(short_filename, '.'); // remove the period

	for (sum = i = 0; i < 11; i++) {
		sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + name[i];
	}

	//This resulting checksum value is stored in each of the LFN entry to ensure that the short filename it points to indeed is the currently 8.3 entry it should be pointing to. 

	free(name);
	
	return sum;
}

int write_entry(fatfs_t *fat, void * entry, unsigned char attr, int loc[])
{
	fat_lfn_entry_t *lfn_entry;
	fat_dir_entry_t *f_entry;
	uint8_t sector[512]; // Each sector is 512 bytes long
	
	if(attr == 0x0F)
	{
		lfn_entry = entry;
	}
	else
	{
		f_entry = entry;
	}
	
	/* Clear out before reading into */
	memset(sector, 0, sizeof(sector));
		
	/* Read it */
	fat->dev->read_blocks(fat->dev, loc[0], 1, sector); 
	
	//printf("Sector BEFORE: %s\n", sector);
	
	/* Memcpy */
	if(attr == 0x0F) // Long file entry
	{
		printf("Writing LFN entry: Order %d\n",lfn_entry->Order);
		memcpy(sector + loc[1] + ORDER, &(lfn_entry->Order), 1);
		memcpy(sector + loc[1] + FNPART1, lfn_entry->FNPart1, 10);
		memcpy(sector + loc[1] + ATTRIBUTE, &(lfn_entry->Attr), 1);
		memcpy(sector + loc[1] + CHECKSUM, &(lfn_entry->Checksum), 1);
		memcpy(sector + loc[1] + FNPART2, lfn_entry->FNPart2, 12);
		memcpy(sector + loc[1] + FNPART3, lfn_entry->FNPart3, 4);
	}
	else             // File/folder entry
	{
		//printf("Writing SFN entry: Name %s Ext: %s\n", f_entry->FileName, f_entry->Ext);
		memcpy(sector + loc[1] + FILENAME, f_entry->FileName, 8);
		memcpy(sector + loc[1] + EXTENSION, f_entry->Ext, 3);
		memcpy(sector + loc[1] + ATTRIBUTE, &(f_entry->Attr), 1);
		memcpy(sector + loc[1] + STARTCLUSTER, &(f_entry->FstClusLO), 2);
		memcpy(sector + loc[1] + FILESIZE, &(f_entry->FileSize), 4);
		
		printf("Just saved:\n Name: %s Extension: %s Attr: %x Cluster: %d Filesize: %d\n", f_entry->FileName, f_entry->Ext, f_entry->Attr, f_entry->FstClusLO, f_entry->FileSize);
	}
	
	//printf("Sector After: %s\n", sector);
	
	/* Write it back */
	fat->dev->write_blocks(fat->dev, loc[0], 1, sector);

	return 0;
}

int *get_free_locations(fatfs_t *fat, node_entry_t *curdir, int num_entries)
{
	int i;
	int j;
	int empty_entry_count = 0;
	int sector_loc;
	int ptr_loc;
	int *locations = malloc(sizeof(int)*2);
	uint8_t sector[512]; // Each sector is 512 bytes long
	cluster_node_t    *cur_cluster = curdir->Data_Clusters;
	
	locations[0] = -1;
	locations[1] = -1;
	
	printf("Trying to create the file in this directory: %s\n", curdir->Name);
	
	/* Dealing with root directory */
	if(curdir->Parent == NULL)
	{
		printf("Creating file in root directory\n");
		for(i = 0; i < fat->root_dir_sectors_num; i++) {
			
			/* Get the sector location */
			sector_loc = fat->root_dir_sec_loc + i;
			
			/* Read it */
			fat->dev->read_blocks(fat->dev, sector_loc, 1, sector); 
			
			ptr_loc = 0;
				
			/* Find the ptr loc */
			for(j = 0; j < fat->boot_sector.bytes_per_sector/32; j++)
			{
				if(sector[ptr_loc] == 0x00 || sector[ptr_loc] == 0xe5)
				{
					empty_entry_count++;
					
					if(empty_entry_count == num_entries)
					{
						printf("Root -- Found location with %d free entries \n", num_entries);
						if(ptr_loc < ((num_entries - 1)* 32)) // corner case: traveling across sectors to fill the case
						{
							locations[0] = sector_loc - 1;  // go to previous 
							locations[1] = fat->boot_sector.bytes_per_sector - (((num_entries - 1)*32) - ptr_loc);
						}
						else
						{
							locations[0] = sector_loc;
							locations[1] = ptr_loc - ((num_entries-1)*32);
						}
						
						goto finish; // OMG!@!!$!!@ Yes, its a goto. Used correctly.
					}
				}
				else
				{
					empty_entry_count = 0; // Restart entry count
				}
				
				ptr_loc +=32;
			}
		}
	}
	else
	{
		printf("Creating file in sub directory\n");
		/* Go through each cluster that belongs to this folder */
		while(cur_cluster != NULL) 
		{
			/* Go through each sector in the cluster */
			for(i = 0; i < fat->boot_sector.sectors_per_cluster; i++)
			{
				/* Get the sector location */
				sector_loc = fat->data_sec_loc + (cur_cluster->Cluster_Num - 2) * fat->boot_sector.sectors_per_cluster + i;
				
				/* Read it */
				fat->dev->read_blocks(fat->dev, sector_loc, 1, sector); 
				
				ptr_loc = 0;
				
				/* Find the ptr loc */
				for(j = 0; j < fat->boot_sector.bytes_per_sector/32; j++)
				{
					if(sector[ptr_loc] == 0x00 || sector[ptr_loc] == 0xe5)
					{
						empty_entry_count++;
						
						if(empty_entry_count == num_entries)
						{
							printf("Other -- Found location with %d free entries \n", num_entries);
							if(ptr_loc < ((num_entries - 1)* 32)) // corner case: traveling across sectors to fill the case
							{
								locations[0] = sector_loc - 1;  // go to previous 
								locations[1] = fat->boot_sector.bytes_per_sector - (((num_entries - 1)*32) - ptr_loc);
							}
							else
							{
								locations[0] = sector_loc;
								locations[1] = ptr_loc - ((num_entries-1)*32);
							}
							
							goto finish; // OMG!@!!$!!@ Yes, its a goto. Used correctly.
						}
					}
					else
					{
						empty_entry_count = 0; // Restart entry count
					}
					
					ptr_loc +=32;
				}
			}
			// Cant go across clusters to fulfill case
			empty_entry_count = 0;  
			cur_cluster = cur_cluster->Next;
		}
	}
	finish:
	
	// Didnt find one? Allocate a new cluster for this folder and set loc accordingly.
	if(locations[0] == -1 && locations[1] == -1)
	{
		cur_cluster = curdir->Data_Clusters;
		
		printf("Couldn't find the free entries. Allocating a Cluster...\n");
		
		while(cur_cluster->Next != NULL)
		{
			cur_cluster = cur_cluster->Next;
		}
		
		cur_cluster->Next = allocate_cluster(fat, curdir->Data_Clusters);
		
		locations[0] = fat->data_sec_loc + ((cur_cluster->Next->Cluster_Num - 2) * fat->boot_sector.sectors_per_cluster);   
		locations[1] = 0;
	}
	
	return locations;
}