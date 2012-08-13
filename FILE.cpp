FILE *fp=NULL;
long len=0;
fp=fopen((LPCSTR)m_FileName,"rb");
fseek(fp,0,SEEK_END);
len=ftell(fp);
fseek(fp,0,SEEK_SET);	 	
UCHAR * lpFileContent=new UCHAR[len];
memset(lpFileContent,0,len);
fread(lpFileContent,1,len,fp);
fclose(fp);

fwrite(strDataPool.c_str(), 1, nPieceLen, fp);
fflush(fp);
----------------------------------------
