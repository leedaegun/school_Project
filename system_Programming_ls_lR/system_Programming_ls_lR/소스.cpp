#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <stdlib.h>
#include  <string.h>
#include <linux/limits.h>

#define NAME_MAX        255
#define PER0    0       
#define PER1    1       
#define PER2    2       
#define PER3    3      
#define PER4    4       
#define PER5    5      
#define PER6    6       
#define PER7    7       
#define PER8    8       
#define PER9    9       
#define PER10   10     

typedef struct Info_state {
	char    permission[11];
	int     linkcount;
	char    userid[20];
	char    groupid[20];
	int     size;
	int     date[3];
	int     time[2];
	char    filename[255];
	char    link[4096];
} Info_state;

typedef struct Bucket
{
	int Num;
	int FileLenth;
	Info_state **bucket;
}Bucket;

void Init_Bucket(Bucket *bk)
{
	bk->Num = 1;
	bk->FileLenth = 0;
	bk->bucket = (Info_state**)malloc(sizeof(Info_state*) * bk->Num + 1);
}





void Push_Bucket(Bucket *bk, Info_state *info)
{
	if (bk->FileLenth == bk->Num)
	{
		int i;
		Info_state **bucket_new;
		bk->Num *= 2;
		bucket_new = (Info_state**)malloc(sizeof(Info_state*) * bk->Num + 1);
		for (i = 0; i < bk->FileLenth; i++)
		{
			bucket_new[i] = bk->bucket[i];
		}
		free(bk->bucket);
		bk->bucket = bucket_new;
	}
	bk->bucket[bk->FileLenth] = info;
	bk->FileLenth++;
}
void Sort(Bucket bk) {
	int i, j;
	Bucket tmp;
	tmp.bucket = (Info_state**)malloc(sizeof(Info_state*));

	for (i = 0; i<bk.FileLenth - 1; i++) {
		Info_state *info = bk.bucket[i];
		for (j = i + 1; j<bk.FileLenth; j++) {
			Info_state *info2 = bk.bucket[j];
			if (strcmp(info->filename, info2->filename) > 0) {
				//strcpy(temp, info->filename);
				//strcpy(info->filename, info2->filename);
				//strcpy(info2->filename, temp);
				tmp.bucket[0] = bk.bucket[i];
				bk.bucket[i] = bk.bucket[j];
				bk.bucket[j] = tmp.bucket[0];
			}
		}
	}

}
int Sum(char filepath[]) //합계 
{
	DIR * dirpt;
	struct dirent * entry;
	int sum = 0;
	struct stat buf;
	char * filename;

	dirpt = opendir(filepath);

	while ((entry = readdir(dirpt)) != NULL)
	{
		if (entry->d_name[0] == '.')
			continue;
		filename = (char *)malloc(sizeof(char) * 1024);
		strcpy(filename, filepath);
		strcat(filename, "/");
		strcat(filename, entry->d_name);
		if ((lstat(filename, &buf)) == 0)
			sum = sum + buf.st_blocks;
	}
	free(filename);
	closedir(dirpt);

	return sum / 2;
}
void CheckFile(struct stat *buf, Info_state * info)
{
	//파일타입검사
	if (S_ISDIR(buf->st_mode))
		info->permission[PER0] = 'd';
	else if (S_ISLNK(buf->st_mode))
		info->permission[PER0] = 'l';
	else if (S_ISCHR(buf->st_mode))
		info->permission[PER0] = 'c';
	else if (S_ISBLK(buf->st_mode))
		info->permission[PER0] = 'b';
	else if (S_ISSOCK(buf->st_mode))
		info->permission[PER0] = 's';
	else if (S_ISFIFO(buf->st_mode))
		info->permission[PER0] = 'P';
	else
		info->permission[PER0] = '-';
	//유저권한검사
	if (buf->st_mode & S_IRUSR)
		info->permission[PER1] = 'r';
	else
		info->permission[PER1] = '-';
	if (buf->st_mode & S_IWUSR)
		info->permission[PER2] = 'w';
	else
		info->permission[PER2] = '-';
	if (buf->st_mode & S_IXUSR)
		info->permission[PER3] = 'x';
	else if (buf->st_mode & S_ISUID)
		info->permission[PER3] = 's';
	else
		info->permission[PER3] = '-';

	//그룹권한검사
	if (buf->st_mode & S_IRGRP)
		info->permission[PER4] = 'r';
	else
		info->permission[PER4] = '-';
	if (buf->st_mode & S_IWGRP)
		info->permission[PER5] = 'w';
	else
		info->permission[PER5] = '-';
	if (buf->st_mode & S_IXGRP)
		info->permission[PER6] = 'x';
	else if (buf->st_mode & S_ISGID)
		info->permission[PER6] = 's';
	else
		info->permission[PER6] = '-';
	//other권한검사
	if (buf->st_mode & S_IROTH)
		info->permission[PER7] = 'r';
	else
		info->permission[PER7] = '-';
	if (buf->st_mode & S_IWOTH)
		info->permission[PER8] = 'w';
	else
		info->permission[PER8] = '-';
	//sticky bit
	if (buf->st_mode & S_IXOTH)
	{
		if (buf->st_mode & S_ISVTX)
			info->permission[PER9] = 't';
		else
			info->permission[PER9] = 'x';
	}
	else
	{
		if (buf->st_mode & S_ISVTX)
			info->permission[PER9] = 'T';
		else
			info->permission[PER9] = '-';
	}

	info->permission[PER10] = '\0';
}

void Print(Bucket *bk)
{
	int i;
	for (i = 0; i <bk->FileLenth; i++) {
		Info_state *info = bk->bucket[i];

		printf("%s", info->permission);
		printf(" %*d ", info->linkcount);
		printf(" %s", info->userid);
		printf(" %s", info->groupid);
		printf(" %d", info->size);
		printf(" %d-%02d-%02d", info->date[0], info->date[1], info->date[2]);
		printf(" %02d:%02d", info->time[0], info->time[1]);
		printf(" %s", info->filename);

		if (info->permission[0] == 'l')
			printf(" -> %s\n", info->link);
		else
			printf("\n");
	}
}

int ls_lR(char dirname[]) {

	Bucket 	bk;
	Info_state * info;
	DIR * dirpt;

	struct dirent * entry;
	struct stat buf;
	struct group * grp;
	struct passwd * pwd;
	struct tm * time;

	char fileName[NAME_MAX];
	char dirPath[NAME_MAX];
	int sum = 0, readc = 0;
	int i;
	Init_Bucket(&bk);

	if ((dirpt = opendir(dirname)) == NULL)
	{
		printf("Directory open fail");
		return -1;
	}


	printf("%s:\n", dirname);
	sum = Sum(dirname);
	printf("합계 %d\n", sum);



	while ((entry = readdir(dirpt)) != NULL)
	{
		strcpy(fileName, dirname);
		strcat(fileName, "/");
		strcat(fileName, entry->d_name);

		if (entry->d_name[0] == '.')
			continue;
		if ((lstat(fileName, &buf) == 0))
		{
			info = (Info_state *)malloc(sizeof(Info_state));
			pwd = getpwuid(buf.st_uid);
			grp = getgrgid(buf.st_gid);
			strcpy(info->userid, pwd->pw_name);//사용자명
			strcpy(info->groupid, grp->gr_name);	//그룹명

			info->linkcount = buf.st_nlink;//파일링크수
			info->size = buf.st_size;//파일 사이즈

			time = localtime(&buf.st_mtime);//접근시간

			info->date[0] = (time->tm_year) + 1900;
			info->date[1] = (time->tm_mon) + 1;
			info->date[2] = time->tm_mday;
			info->time[0] = time->tm_hour;
			info->time[1] = time->tm_min;

			CheckFile(&buf, info);//권한검사
			strcpy(info->filename, entry->d_name);

			if (S_ISLNK(buf.st_mode))
			{
				readc = readlink(fileName, info->link, sizeof(info->link));
				info->link[readc] = NULL;
			}

			Push_Bucket(&bk, info);
		}
		else
			printf("can't read file stat");

	}
	//정렬
	Sort(bk);
	//출력
	Print(&bk);

	//재귀		
	for (i = 0; i<bk.FileLenth; i++) {

		Info_state *info = bk.bucket[i];
		if (info->permission[0] == 'd'&& info->filename[0] != '.') {

			printf("\n");
			strcpy(dirPath, dirname);
			strcat(dirPath, "/");
			strcat(dirPath, info->filename);
			ls_lR(dirPath);
		}
	}

	closedir(dirpt);
	return 0;


}

int main() {


	if (ls_lR(".") == -1) {
		printf("fail");
		return -1;
	}


	return 0;
}

