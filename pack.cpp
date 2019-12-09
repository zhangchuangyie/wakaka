
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<io.h>
#include<iostream>
#include<string>


using namespace std;
 int count=0;

	typedef struct Note{
		char path[50];
		struct Note *next;
	}Note,*Lnote;

	typedef struct Node {
		char name[20];
		unsigned int size;

		struct Node *next;    
	}Node,*List;
	
	List creat(void);
	void add(List *head,char *Tname,int Tsize);
	void bwrite(char *fn); 
	void file_in(char *Path_r,char *Path_w);
	void unzip(char *Path_file,char *Path_out);
	void lookthepack(char *path);
	Lnote Tcreat(void);
	void Tadd(Lnote *Thead,char *Path);
	void zip(char *ar1,char *ar2);	
	
	
	
	void lookthepack(char *path){
		int i,j;
		Node p;
		FILE *fp;
			if((fp=fopen(path,"rb"))==NULL){
			printf("Can't open %s\n",path);
				exit(1);
			}
			fseek(fp,0L,SEEK_SET);	
			rewind(fp);
			fread(&j,sizeof(j),1,fp);
			printf("打包文件总数为：  %d\n",j);
			for(i=0;i<j;i++){
				fread(&p,sizeof(p),1,fp);
				printf("%s\n",p.name);
			}
	}
	
	
	
	void zip(char *ar1,char *ar2){
			char check;
			int l;
			List head=creat();
			Lnote Thead=Tcreat();
	  		long Handle;
			struct _finddata_t FileInfo;
				l=strlen(ar1);
			for(;l>0;l--){
				if(ar1[l-1]=='\\'){
					check=ar1[l];
					break;}
			}
			if(check!='*')
				strcat(ar1,"\\*.*");		
					
		if((Handle=_findfirst(ar1,&FileInfo))==-1){
		/*  以下为路径不包括通配符情况*/
			printf("路径不存在！\n");
			exit(0);
	}
		
		else{
		do{
			if (!(strcmp(FileInfo.name, "..")&&strcmp(FileInfo.name,"."))){
				continue;
				}
		char path_a[40];
/*	以下旨在获取除去通配符后的文件路径	*/ 
		int str_leng,t;
		strcpy(path_a,ar1);
		str_leng=strlen(path_a);
		for(t= str_leng+1;;t--)
			{if(path_a[t-1]=='\\'){
				path_a[t]='\0';
				break;}
			else
				path_a[t]='0';		
				
			}
			
		char Tname[20];
		strcpy(Tname,FileInfo.name);
			strcat(path_a,Tname);		//获取绝对路径 

				
			Tadd(&Thead,path_a);
			add(&head,FileInfo.name,FileInfo.size);      
			 count++;		 
			}while(_findnext(Handle,&FileInfo)==0);	 
		_findclose(Handle);

		
		FILE *fp_stu,*fp_txt;
		if((fp_stu=fopen(ar2,"ab+"))==NULL){
			printf("Can't open++++ %s\n",ar2);
			exit(1);
		}
		else{
			rewind(fp_stu);
			fwrite(&count,sizeof(count),1,fp_stu);		//记录文件总个数为int型，并放在打包文件头 

			Node *op;

			op=head->next;
			while(op!=NULL){
				fwrite(op,sizeof(*op),1,fp_stu);	
				op=op->next;
				}
				fclose(fp_stu);	
		}	
	
		
		Note *pt;
		pt=Thead->next;
		while(pt!=NULL){
			file_in(pt->path,ar2);     //文件读取及写入 
			pt=pt->next;
		}	
	}
}   

	
	void unzip(char *Path_file,char *Path_out){
		FILE *fp1,*fp2,*fp3;
		char ch;
		int num,m=0;
		Node p;
		
		if((fp1=fopen(Path_file,"rb"))==NULL){
			printf("Can't open %s\n",Path_file);
				exit(1);
			}
		else if((fp2=fopen(Path_file,"rb"))==NULL){
			printf("Can't open %s\n",Path_file);
				exit(1);
			}	
		else{
		
		fseek(fp1,0L,SEEK_SET);	
		fseek(fp2,0L,SEEK_SET);		
		rewind(fp1);
		rewind(fp1);
		fread(&num,sizeof(num),1,fp1);
		printf("The file-count is   :    %d\n",num);
		fseek(fp2,sizeof(int),SEEK_SET);
		fseek(fp2,sizeof(p)*num,SEEK_CUR);
		do {	m++;
		fread(&p,sizeof(p),1,fp1);
		char fullpath_out[40];
		char name[20];
		int i=0;
		strcpy(name,p.name);
		strcpy(fullpath_out,Path_out);
		strcat(fullpath_out,name);
		if((fp3=fopen(fullpath_out,"ab+"))==NULL){
			printf("Can't open    %s   \n",fullpath_out);
				exit(1);
			}
		else{
			
			for(i=0;i<p.size;i+=(sizeof(char))){
			fread(&ch,sizeof(ch),1,fp2);
			fwrite(&ch,sizeof(ch),1,fp3);
			}
		 }
		}while(m<num);
			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
	
	}
	  
}
	  
	void file_in(char *Path_r,char *Path_w){
		FILE *fpr,*fpw;
		char ch;
		if((fpr=fopen(Path_r,"rb"))==NULL){
			printf("Can't open  文件路径  %s\n",Path_r);
				exit(1);
			}
			
		if((fpw=fopen(Path_w,"ab+"))==NULL){
			printf("Can't open  打包指向路径 %s\n",Path_w);
			exit(1);
			}
			fseek(fpw,0L,SEEK_END);				
			while ((ch=fgetc(fpr))!=EOF)
				fputc(ch,fpw);
		
		
			fclose(fpr);
			fclose(fpw);		
	}
	
	Lnote Tcreat(void){
		Lnote Thead=(Lnote)malloc(sizeof(Note));
		Thead->next=NULL;
		return Thead;
	}
	void Tadd(Lnote *Thead,char *Path){
		Note *s,*r;
		r=*Thead;
		while(r->next!=NULL){
			r=r->next;		
		}
		s=(Lnote)malloc(sizeof(Note));
		strcpy(s->path,Path);
		r->next=s;
		s->next=NULL;
	}
	
	List creat(void){
		List head=(List)malloc(sizeof(Node));
		head->next=NULL;
		return head;
}
	void add(List *head,char *Tname,int Tsize){
		int i=0;
		Node *s,*r;
		r=*head;
	 
		while(r->next!=NULL){
			r=r->next;		
		}	
		s=(List)malloc(sizeof(Node));
		strcpy(s->name,Tname);
		s->size=Tsize;
		r->next=s;
		s->next=NULL;
	} 







int main(int argc,char *argv[]){
	
		
	if(argc>=1){
		if(strcmp(argv[1],"help")==0){
    	printf("此程序命令行参数如下：  \n");
	    printf("命令总数为 %d    \n",argc+1);  
		for(int i=0;i<argc;i++)
    		printf("%s     %d\n",argv[i],i);
    	}
	}

	/*  以下对应于argv[1]为‘-l’时，表示显示压缩包内的文件 */	
   
    
    char a[20],b[5];
	for(int i=0;i<2;i++){
		b[i]=argv[1][i];
	} 
		b[2]='\0';
	 
    if(strcmp(argv[1],"-l")==0){
   		lookthepack(argv[2]); 	
    	
	}	
	
	/*  以下对应于argv[1]为‘-u’时，表示解压 */	
   	else if(strcmp(b,"-u")==0){
   		printf("sasda\n"); 
   		unzip(argv[2],argv[3]);
	   }
		
	else {	
			zip(argv[1],argv[2]);			
	}
		return 0;
		
//E:\打包事例\test.exe E:\打包事例\Thepack\*.*
//E:\打包事例\test.exe -l E:\打包事例\testqq.txt
//E:\打包事例\test.exe -u E:\打包事例\testqq.txt E:\打包事例\zxcv 


	
}
