//Uygulama ile �lgili Genel A��klamalar:Berkan K�M�RC�
//160757038
//Bilgisayar M�hendisli�i(��) 
//Paralel Programlama 
//BM-308-50-50
//2019 - 2020 Akademik Y�l� Bahar D�nemi
//Proje 3
//Dr.��r.�yesi Deniz DAL


//K�t�phane Denklarasyonu
#include <iostream>
#include <mpi.h>//mpi k�t�phanesi
#include <fstream>//dosya i�lemleri i�in 
#include <sys/time.h>//zaman hesab� i�in 
#include <cstdlib>
#include <ctime>
#include <string>

//Fonksiyon Protetipleri=Program�m tek bir main fonksiyonu ile �al��maktad�r

using namespace std;
//Main fonksiyonu 
int main(int argc, char* argv[]) {//main fonksiyonunu a�t�k
	  double startTime,endTime,elapsedTime;//zaman hesab� i�in de�i�kenlerimizi tan�mlad�k
	  int * goruntuMatrisiPointer;//filtrelenmemi� matrisin tutulca�� tek boyutlu pointer dizi(�zerinde filtre uygulucaz.)
	  int * yedekGoruntuMatrisiPointer;//bu diziyi yedeklice�imiz yine tek boyutlu bir pointer dizi
	  int *lokalDiziPointer;//diziyi toplu haberle�me ile da��tt�ktan sonra �zerinde filtre uygulayaca��m�z dizi
	  int *lokalDiziPointer2;//diziyi toplu haberle�me ile da��tt�ktan sonra o dizinin yede�ini alaca��m�z tek boyutlu bir pointer dizi
      int kismiSatirBoyutu,kismiToplamBoyut;//sat�rlar�n b�l�mlenmi� de�erlerini ve toplam boyutun b�l�mlenmi� de�erlerini saklad���m de�i�kenler
	  int geciciDegisken;//s�ralamada kullaca��m�z de�i�ken
	  int siralamaMatrisi[25];//s�ralaamay� saklad���m�z tek boyutlu normal dizi
      int satirlar;//satirlar degiskeni
	  int sutunlar;//sutunlar degiskeni
	  int myRank;//myRank de�i�keni
	  int size;//size de�i�keni
	  MPI_Status status;//status sabiti
	  MPI_Init(&argc,&argv);//program� ilklendirdik
	  MPI_Comm_rank(MPI_COMM_WORLD,&myRank);//rank bilgisini (yani rank bilgisi hangi bilgisauyarda oldu�umuz) ald�k
	  MPI_Comm_size(MPI_COMM_WORLD,&size);//size bilgisini ald�k bilgisayarlar�n boyutu yani ka� adet oldu�u.
	  
if(myRank==0)//e�er master bilgisayar�n i�indeysek.
{
	  	ifstream okunacakDosya;//okunacakdosya ad�nda i�inde veri alabildi�imiz bir dosya olu�turduk ifstream ile
	  	okunacakDosya.open(argv[1]);//dosyay� a�t�k;argv1 ile a�t�k ��nk� �al��t�r�rken komut sat�r�ndan girdi�imiz ilk parametre argv1 olucak  
	  	okunacakDosya>>satirlar;//satirlar de�i�kenine okunacakDosya ad�ndaki dosyan�n ilk verisini kaydettik
	    okunacakDosya>>sutunlar;//sutunlar de�i�kenine okunacakDosya ad�ndaki dosyan�n ikinci verisini kaydettik
	    int boyut=satirlar*sutunlar;//boyut de�i�kenini hesaplad�k
	    goruntuMatrisiPointer = new int[boyut];//goruntuMatrisiPointer � yukarda tan�mlam��t�k burdada tek boyutlu bir dinamik dizi haaline getirdik.
	    yedekGoruntuMatrisiPointer = new int[boyut];//yine ayn� �ekilde yedekGoruntuMatrisiPointer � yukarda tan�mlam��t�k burdada tek boyutlu bir dinamik dizi haaline getirdik
	    int atanacakIndisler;//goruntu matrisinin i�ine ataca��m�z indisler i�in bir de�i�ken tan�mlad�k
	    int matrisinIndisi=0;//matris indisini 0 dan ba�latt�k
            for(int s1=0;s1<satirlar;s1++)//satirlarin hepsini dola�acak d��taki d�ng�y� tan�mlad�k
            {
    	      for(int s2=0;s2<sutunlar;s2++)//s�tunlar�n hepsini dola��cak i�teki d�ng�y� tan�mlad�k
    	      {
    	        okunacakDosya>>atanacakIndisler;//atanacak indisler de�i�kenini ald�k bunu sat�r ve s�tunun yaz�l� oldu�u sat�rdan de�ilde bir alt sat�rdan ba�lamas�
    	        //i�in yapt�k  bu her okundu�unda bir artarak devam edicek yani matristeki her eleman�n bir sa��ndaki indisi al�cak hep
    	        goruntuMatrisiPointer[matrisinIndisi]=atanacakIndisler;//o elemanlar�da matrisin 0 dan sat�r ve s�tun say�s� �arp�m�na kadar b�t�n indislerine at�cak
    	        yedekGoruntuMatrisiPointer[matrisinIndisi]=goruntuMatrisiPointer[matrisinIndisi];//Diziyi bir baa�ka dizide yedekliyoruz.
    	        matrisinIndisi++;//matrisin indisini 1 artt�r�yoruz her iterasyonda
	          }
            }
        okunacakDosya.close();//okuma i�lemlerimiz bittikten sonra dosyam�z� kapat�yoruz
        startTime=MPI_Wtime();//S�reyi hesaplamak i�in MPI_Wtime komutunu �a��r�yoruz.S�reyi hesaplamaya ba�l�cak
	  
	   
		kismiSatirBoyutu=(satirlar/size);//sat�r boyutunu b�lerek bir de�i�kende tutmam�z laz�mki her bilgisayar kendisinin filtre yapt��� sat�r say�s�n� bilmeli.
        kismiToplamBoyut=(satirlar/size)*sutunlar;//ayn� zamanda toplamboyutunda b�l�nm�� de�erini bulmam gerekiyo ki master bilgisayardan di�erlerine bu de�eri atmam gerekicek.
        	//��nk� her bilgisayar kendisine ayr�lm�� k�smiboyutu bir dizide de�erleriyle birlikte(filtrelenmemi� matrisin) saklamas� gerekicek ki filtreleme yapabilsin her bilgisayar
	    
}
         
	        MPI_Bcast(&kismiSatirBoyutu,1,MPI_INT,0,MPI_COMM_WORLD);
	        MPI_Bcast(&sutunlar,1,MPI_INT,0,MPI_COMM_WORLD);
	        kismiToplamBoyut=kismiSatirBoyutu*sutunlar;//k�smitoplamboyutu tekrar hesapl�yoruz burda ge�erli olsun diye
            lokalDiziPointer=new int[kismiToplamBoyut];//k�smitoplamboyutunda bir dinamik dizi tan�ml�yoruz 
            lokalDiziPointer2=new int[kismiToplamBoyut];//yede�ini almak i�in ayn� �ekilde bir dinamik dizi daha tan�ml�yoruz bunlar� yukarda denklare etmi�tik
	        MPI_Scatter(yedekGoruntuMatrisiPointer,kismiToplamBoyut,MPI_INT,lokalDiziPointer,kismiToplamBoyut,MPI_INT,0,MPI_COMM_WORLD);//veriyi(filtrelenmemi� verinin k�smi kadar boyutunu 
			//scatter ile payla�t�r�yoruz.
	        for(int i=0;i<kismiToplamBoyut;i++)//yedek almak i�in for a��yoruz
	        {
		      lokalDiziPointer2[i]=lokalDiziPointer[i];//verimizi yedekliyoruz ayn� boyutta bir dinamik dizi i�erisine
		    }
		    for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//indisi 2 den ba�lat�p k�smiSat�rBoyutunun bir eksi�ine g�nderiyoruz b�ylece ilk iki sat�r� ve sok iki sat�r� alm�cak
			{
			for(int s2=2;s2<sutunlar-1;s2++)//s�tunlar�nda 1 eksi�ine kadar gidiyoruz son iki s�tunu ve ilk iki s�tunu almamas� i�in he bide <= de�i < dikkat. d��taki fordada �lyle
			{
					 //s�ralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulaca��m�z indisleri yani komu�u de�erlerini at�yoruz. Detayl� a��klamas�n�
					 //ilk projede yapm��t�m 
					 siralamaMatrisi[1]=lokalDiziPointer[(s1-2)*sutunlar+s2-1];
	   	             siralamaMatrisi[2]=lokalDiziPointer[(s1-2)*sutunlar+s2];
	   	             siralamaMatrisi[3]=lokalDiziPointer[(s1-2)*sutunlar+s2+1];
	   	             siralamaMatrisi[4]=lokalDiziPointer[(s1-2)*sutunlar+s2+2];
	   	             siralamaMatrisi[5]=lokalDiziPointer[(s1-1)*sutunlar+s2-2];
	   	             siralamaMatrisi[6]=lokalDiziPointer[(s1-1)*sutunlar+s2-1];
	   	             siralamaMatrisi[7]=lokalDiziPointer[(s1-1)*sutunlar+s2];
	   	             siralamaMatrisi[8]=lokalDiziPointer[(s1-1)*sutunlar+s2+1];
	   	             siralamaMatrisi[9]=lokalDiziPointer[(s1-1)*sutunlar+s2+2];
	   	             siralamaMatrisi[10]=lokalDiziPointer[(s1)*sutunlar+s2-2];
	   	             siralamaMatrisi[11]=lokalDiziPointer[(s1)*sutunlar+s2-1];
	   	             siralamaMatrisi[12]=lokalDiziPointer[(s1)*sutunlar+s2];
	   	             siralamaMatrisi[13]=lokalDiziPointer[(s1)*sutunlar+s2+1];
	   	             siralamaMatrisi[14]=lokalDiziPointer[(s1)*sutunlar+s2+2];
	   	             siralamaMatrisi[15]=lokalDiziPointer[(s1+1)*sutunlar+s2-2];
	   	             siralamaMatrisi[16]=lokalDiziPointer[(s1+1)*sutunlar+s2-1];
	   	             siralamaMatrisi[17]=lokalDiziPointer[(s1+1)*sutunlar+s2];
	   	             siralamaMatrisi[18]=lokalDiziPointer[(s1+1)*sutunlar+s2+1];
	   	             siralamaMatrisi[19]=lokalDiziPointer[(s1+1)*sutunlar+s2+2];
	   	             siralamaMatrisi[20]=lokalDiziPointer[(s1+2)*sutunlar+s2-2];
	   	             siralamaMatrisi[21]=lokalDiziPointer[(s1+2)*sutunlar+s2-1];
	   	             siralamaMatrisi[22]=lokalDiziPointer[(s1+2)*sutunlar+s2];
	   	             siralamaMatrisi[23]=lokalDiziPointer[(s1+2)*sutunlar+s2+1];
	   	             siralamaMatrisi[24]=lokalDiziPointer[(s1+2)*sutunlar+s2+2];
					 //bubble sort  algoritmas�yla k���kten b�y��e s�ralama yap�yoruz 	
					 for(int i=0;i<25;i++)
	   	             {
	   	     	      for(int j=i;j<25;j++)
	   	 	          {
	   	 		       if(siralamaMatrisi[i]>siralamaMatrisi[j])
	   	 		       {
	   	 		          geciciDegisken=siralamaMatrisi[i];
	   	 		          siralamaMatrisi[i]=siralamaMatrisi[j];
	   	 		          siralamaMatrisi[j]=geciciDegisken;
	   	 	           }
			          }
	   	 	
		              }	
					lokalDiziPointer2[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca de�eri ilgili indise at�yoruz
				  }
		 	}
		
	        MPI_Gather(lokalDiziPointer2,kismiToplamBoyut,MPI_INT,yedekGoruntuMatrisiPointer,kismiToplamBoyut,MPI_INT,0,MPI_COMM_WORLD);//g�nderilen veriyi d�nyadaki b�t�n bilgisayarlar al�cak
	        
	       /* if(myRank==0)
	        {
	        	for(int s1=satirlar-5;s1<satirlar-4;s1++)//indisi 2 den ba�lat�p k�smiSat�rBoyutunun bir eksi�ine g�nderiyoruz b�ylece master bilgisayar ilk iki sat�r� ve sok iki sat�r� alm�cak
			{
				for(int s2=2;s2<sutunlar-1;s2++)//s�tunlar�nda 1 eksi�ine kadar gidiyoruz son iki s�tunu ve ilk iki s�tunu almamas� i�in he bide <= de�i < dikkat. d��taki fordada �lyle
				{
					 //s�ralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulaca��m�z indisleri yani komu�u de�erlerini at�yoruz. Detayl� a��klamas�n�
					 //ilk projede yapm��t�m 
					 siralamaMatrisi[0]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2-2];
	   	             siralamaMatrisi[1]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2-1];
	   	             siralamaMatrisi[2]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2];
	   	             siralamaMatrisi[3]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2+1];
	   	             siralamaMatrisi[4]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2+2];
	   	             siralamaMatrisi[5]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2-2];
	   	             siralamaMatrisi[6]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2-1];
	   	             siralamaMatrisi[7]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2];
	   	             siralamaMatrisi[8]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2+1];
	   	             siralamaMatrisi[9]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2+2];
	   	             siralamaMatrisi[10]=goruntuMatrisiPointer[(s1)*sutunlar+s2-2];
	   	             siralamaMatrisi[11]=goruntuMatrisiPointer[(s1)*sutunlar+s2-1];
	   	             siralamaMatrisi[12]=goruntuMatrisiPointer[(s1)*sutunlar+s2];
	   	             siralamaMatrisi[13]=goruntuMatrisiPointer[(s1)*sutunlar+s2+1];
	   	             siralamaMatrisi[14]=goruntuMatrisiPointer[(s1)*sutunlar+s2+2];
	   	             siralamaMatrisi[15]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2-2];
	   	             siralamaMatrisi[16]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2-1];
	   	             siralamaMatrisi[17]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2];
	   	             siralamaMatrisi[18]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2+1];
	   	             siralamaMatrisi[19]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2+2];
	   	             siralamaMatrisi[20]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2-2];
	   	             siralamaMatrisi[21]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2-1];
	   	             siralamaMatrisi[22]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2];
	   	             siralamaMatrisi[23]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2+1];
	   	             siralamaMatrisi[24]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2+2];
					 //bubble sort  algoritmas�yla k���kten b�y��e s�ralama yap�yoruz 	
					 for(int i=0;i<25;i++)
	   	             {
	   	     	      for(int j=i;j<25;j++)
	   	 	          {
	   	 		       if(siralamaMatrisi[i]>siralamaMatrisi[j])
	   	 		       {
	   	 		          geciciDegisken=siralamaMatrisi[i];
	   	 		          siralamaMatrisi[i]=siralamaMatrisi[j];
	   	 		          siralamaMatrisi[j]=geciciDegisken;
	   	 	           }
			          }
	   	 	
		              }	
					yedekGoruntuMatrisiPointer[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca de�eri ilgili indize at�yoruz
				  }
		 	}	
	        	
			}*/
	        if(myRank==0)//ve rank� 0 olan bilgisayarlar
	        {
	        	endTime=MPI_Wtime();	//hesaplama s�resini bitiriyoruz
		        elapsedTime=endTime-startTime;//aradaki farkl� al�p bir de�i�kene at�yoruz
		        cout<<"Paralel Programlama Suresi: "<<elapsedTime*1000<<endl;//o de�i�keni de ilgili formatta yazd�r�yoruz.
	    
		
	        char ciktiDosyasi[28];//Filtrelenmi� matrisin ��kt� halini char dizisi �eklinde tan�mlad�k �rn:GoruntuMatrisi1
    	    string ciktiDosyasiUzantisi= "_filtered.txt";//arg�man�n sonuna uzant� eklemek i�in de�i�ken tan�mlad�k
    	    for(int s1=0; s1<15; s1++)//0 dan 15 e kadar gidicek for 
	        {
	          ciktiDosyasi[s1]=argv[1][s1+22];// arg�man�n toplam (.txt hari�) 38 karakteri var biz sadece GoruntuMatrisi1 �rne�i �eklinde 15 karakterli bir de�er atad�k	
	        }
	        for(int s1=0; s1<14; s1++)//sonra o 15 de�erli karakterin sonuna 15 ten ba�layarak(15 ten alttaki forda ba�l�yor) (devam� altta ki a��klama sat�r�nda)
	        {
	          ciktiDosyasi[s1+15] = ciktiDosyasiUzantisi[s1];//uzant�y� atad�k art�k i�inde GoruntuMatrisi1_filtered.txt �rne�i �eklinde bir de�er olmu� oldu
	        } 								
		
	        ofstream yazilacakDosya;//yazilacak dosyay� ��kt� dosyas� olarak ayarlad�k
	        yazilacakDosya.open(ciktiDosyasi, ios::out);//burda char dizisini �evirme i�lemi ve yaz�lacak dosyay� a�ma i�lemi yap�yoruz
            //yazilacakDosya<<satirlar;
            //yazilacakDosya<<sutunlar;
	        for(int s1=0; s1<satirlar; s1++)//sat�rlara kadar giden bir d�ng�
            {
               for(int s2=0; s2<sutunlar; s2++)//s�tunlara kadar giden bir d�ng�
	           {
		           yazilacakDosya<<yedekGoruntuMatrisiPointer[s1*sutunlar+s2]<<"\t";//yaz�lacak matris de�erleri matrisin ilk sat�r ve ilk s�tunundan ba�l�cak
                  //dikkat et matrisin sat�r ve s�tun bilgisinin yazd��� sat�r� alm�cak onu almak isteseydik bu sat�r�n �st�ndeki 7.ve 8.s�tunu
                  //sat� ve s�tun say�s� bilgisini yazd�rd���m�z k�sm� a��klama sat�r�ndan kald�rmam�z gerekirdi
					
	           }
	           yazilacakDosya<<endl;//matris format�nda yazd�rmak i�in her sat�rdan sonra bir alta iniyoruz.
	        }	

	    yazilacakDosya.close();//yaz�lacak dosyay� kapatt�k 
	    delete [] yedekGoruntuMatrisiPointer;//di�er bilgisayarlara veri g�ndermek i�in kulland���m�z dinamikdiziyide sildik.
	    delete [] goruntuMatrisiPointer;//dinamnik diziyi sildik(filtreleme yapt���m�z)
	   
	}
	
	    delete [] lokalDiziPointer;//dinamik dizimizi silliyoruz
	    delete [] lokalDiziPointer2;//dinamik dizimizi siliyoruz
	  
	    
        MPI_Finalize();//MPI sonland�r�yoruz
    return 0;//program� sonland�r�yoruz
}
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  

