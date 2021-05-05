//Uygulama ile Ýlgili Genel Açýklamalar:Berkan KÖMÜRCÜ
//160757038
//Bilgisayar Mühendisliði(ÝÖ) 
//Paralel Programlama 
//BM-308-50-50
//2019 - 2020 Akademik Yýlý Bahar Dönemi
//Proje 3
//Dr.Öðr.Üyesi Deniz DAL


//Kütüphane Denklarasyonu
#include <iostream>
#include <mpi.h>//mpi kütüphanesi
#include <fstream>//dosya iþlemleri için 
#include <sys/time.h>//zaman hesabý için 
#include <cstdlib>
#include <ctime>
#include <string>

//Fonksiyon Protetipleri=Programým tek bir main fonksiyonu ile çalýþmaktadýr

using namespace std;
//Main fonksiyonu 
int main(int argc, char* argv[]) {//main fonksiyonunu açtýk
	  double startTime,endTime,elapsedTime;//zaman hesabý için deðiþkenlerimizi tanýmladýk
	  int * goruntuMatrisiPointer;//filtrelenmemiþ matrisin tutulcaðý tek boyutlu pointer dizi(üzerinde filtre uygulucaz.)
	  int * yedekGoruntuMatrisiPointer;//bu diziyi yedekliceðimiz yine tek boyutlu bir pointer dizi
	  int *lokalDiziPointer;//diziyi toplu haberleþme ile daðýttýktan sonra üzerinde filtre uygulayacaðýmýz dizi
	  int *lokalDiziPointer2;//diziyi toplu haberleþme ile daðýttýktan sonra o dizinin yedeðini alacaðýmýz tek boyutlu bir pointer dizi
      int kismiSatirBoyutu,kismiToplamBoyut;//satýrlarýn bölümlenmiþ deðerlerini ve toplam boyutun bölümlenmiþ deðerlerini sakladýðým deðiþkenler
	  int geciciDegisken;//sýralamada kullacaðýmýz deðiþken
	  int siralamaMatrisi[25];//sýralaamayý sakladýðýmýz tek boyutlu normal dizi
      int satirlar;//satirlar degiskeni
	  int sutunlar;//sutunlar degiskeni
	  int myRank;//myRank deðiþkeni
	  int size;//size deðiþkeni
	  MPI_Status status;//status sabiti
	  MPI_Init(&argc,&argv);//programý ilklendirdik
	  MPI_Comm_rank(MPI_COMM_WORLD,&myRank);//rank bilgisini (yani rank bilgisi hangi bilgisauyarda olduðumuz) aldýk
	  MPI_Comm_size(MPI_COMM_WORLD,&size);//size bilgisini aldýk bilgisayarlarýn boyutu yani kaç adet olduðu.
	  
if(myRank==0)//eðer master bilgisayarýn içindeysek.
{
	  	ifstream okunacakDosya;//okunacakdosya adýnda içinde veri alabildiðimiz bir dosya oluþturduk ifstream ile
	  	okunacakDosya.open(argv[1]);//dosyayý açtýk;argv1 ile açtýk çünkü çalýþtýrýrken komut satýrýndan girdiðimiz ilk parametre argv1 olucak  
	  	okunacakDosya>>satirlar;//satirlar deðiþkenine okunacakDosya adýndaki dosyanýn ilk verisini kaydettik
	    okunacakDosya>>sutunlar;//sutunlar deðiþkenine okunacakDosya adýndaki dosyanýn ikinci verisini kaydettik
	    int boyut=satirlar*sutunlar;//boyut deðiþkenini hesapladýk
	    goruntuMatrisiPointer = new int[boyut];//goruntuMatrisiPointer ý yukarda tanýmlamýþtýk burdada tek boyutlu bir dinamik dizi haaline getirdik.
	    yedekGoruntuMatrisiPointer = new int[boyut];//yine ayný þekilde yedekGoruntuMatrisiPointer ý yukarda tanýmlamýþtýk burdada tek boyutlu bir dinamik dizi haaline getirdik
	    int atanacakIndisler;//goruntu matrisinin içine atacaðýmýz indisler için bir deðiþken tanýmladýk
	    int matrisinIndisi=0;//matris indisini 0 dan baþlattýk
            for(int s1=0;s1<satirlar;s1++)//satirlarin hepsini dolaþacak dýþtaki döngüyü tanýmladýk
            {
    	      for(int s2=0;s2<sutunlar;s2++)//sütunlarýn hepsini dolaþýcak içteki döngüyü tanýmladýk
    	      {
    	        okunacakDosya>>atanacakIndisler;//atanacak indisler deðiþkenini aldýk bunu satýr ve sütunun yazýlý olduðu satýrdan deðilde bir alt satýrdan baþlamasý
    	        //için yaptýk  bu her okunduðunda bir artarak devam edicek yani matristeki her elemanýn bir saðýndaki indisi alýcak hep
    	        goruntuMatrisiPointer[matrisinIndisi]=atanacakIndisler;//o elemanlarýda matrisin 0 dan satýr ve sütun sayýsý çarpýmýna kadar bütün indislerine atýcak
    	        yedekGoruntuMatrisiPointer[matrisinIndisi]=goruntuMatrisiPointer[matrisinIndisi];//Diziyi bir baaþka dizide yedekliyoruz.
    	        matrisinIndisi++;//matrisin indisini 1 arttýrýyoruz her iterasyonda
	          }
            }
        okunacakDosya.close();//okuma iþlemlerimiz bittikten sonra dosyamýzý kapatýyoruz
        startTime=MPI_Wtime();//Süreyi hesaplamak için MPI_Wtime komutunu çaðýrýyoruz.Süreyi hesaplamaya baþlýcak
	  
	   
		kismiSatirBoyutu=(satirlar/size);//satýr boyutunu bölerek bir deðiþkende tutmamýz lazýmki her bilgisayar kendisinin filtre yaptýðý satýr sayýsýný bilmeli.
        kismiToplamBoyut=(satirlar/size)*sutunlar;//ayný zamanda toplamboyutunda bölünmüþ deðerini bulmam gerekiyo ki master bilgisayardan diðerlerine bu deðeri atmam gerekicek.
        	//çünkü her bilgisayar kendisine ayrýlmýþ kýsmiboyutu bir dizide deðerleriyle birlikte(filtrelenmemiþ matrisin) saklamasý gerekicek ki filtreleme yapabilsin her bilgisayar
	    
}
         
	        MPI_Bcast(&kismiSatirBoyutu,1,MPI_INT,0,MPI_COMM_WORLD);
	        MPI_Bcast(&sutunlar,1,MPI_INT,0,MPI_COMM_WORLD);
	        kismiToplamBoyut=kismiSatirBoyutu*sutunlar;//kýsmitoplamboyutu tekrar hesaplýyoruz burda geçerli olsun diye
            lokalDiziPointer=new int[kismiToplamBoyut];//kýsmitoplamboyutunda bir dinamik dizi tanýmlýyoruz 
            lokalDiziPointer2=new int[kismiToplamBoyut];//yedeðini almak için ayný þekilde bir dinamik dizi daha tanýmlýyoruz bunlarý yukarda denklare etmiþtik
	        MPI_Scatter(yedekGoruntuMatrisiPointer,kismiToplamBoyut,MPI_INT,lokalDiziPointer,kismiToplamBoyut,MPI_INT,0,MPI_COMM_WORLD);//veriyi(filtrelenmemiþ verinin kýsmi kadar boyutunu 
			//scatter ile paylaþtýrýyoruz.
	        for(int i=0;i<kismiToplamBoyut;i++)//yedek almak için for açýyoruz
	        {
		      lokalDiziPointer2[i]=lokalDiziPointer[i];//verimizi yedekliyoruz ayný boyutta bir dinamik dizi içerisine
		    }
		    for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//indisi 2 den baþlatýp kýsmiSatýrBoyutunun bir eksiðine gönderiyoruz böylece ilk iki satýrý ve sok iki satýrý almýcak
			{
			for(int s2=2;s2<sutunlar-1;s2++)//sütunlarýnda 1 eksiðine kadar gidiyoruz son iki sütunu ve ilk iki sütunu almamasý için he bide <= deði < dikkat. dýþtaki fordada ölyle
			{
					 //sýralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulacaðýmýz indisleri yani komuþu deðerlerini atýyoruz. Detaylý açýklamasýný
					 //ilk projede yapmýþtým 
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
					 //bubble sort  algoritmasýyla küçükten büyüðe sýralama yapýyoruz 	
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
					lokalDiziPointer2[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca deðeri ilgili indise atýyoruz
				  }
		 	}
		
	        MPI_Gather(lokalDiziPointer2,kismiToplamBoyut,MPI_INT,yedekGoruntuMatrisiPointer,kismiToplamBoyut,MPI_INT,0,MPI_COMM_WORLD);//gönderilen veriyi dünyadaki bütün bilgisayarlar alýcak
	        
	       /* if(myRank==0)
	        {
	        	for(int s1=satirlar-5;s1<satirlar-4;s1++)//indisi 2 den baþlatýp kýsmiSatýrBoyutunun bir eksiðine gönderiyoruz böylece master bilgisayar ilk iki satýrý ve sok iki satýrý almýcak
			{
				for(int s2=2;s2<sutunlar-1;s2++)//sütunlarýnda 1 eksiðine kadar gidiyoruz son iki sütunu ve ilk iki sütunu almamasý için he bide <= deði < dikkat. dýþtaki fordada ölyle
				{
					 //sýralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulacaðýmýz indisleri yani komuþu deðerlerini atýyoruz. Detaylý açýklamasýný
					 //ilk projede yapmýþtým 
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
					 //bubble sort  algoritmasýyla küçükten büyüðe sýralama yapýyoruz 	
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
					yedekGoruntuMatrisiPointer[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca deðeri ilgili indize atýyoruz
				  }
		 	}	
	        	
			}*/
	        if(myRank==0)//ve ranký 0 olan bilgisayarlar
	        {
	        	endTime=MPI_Wtime();	//hesaplama süresini bitiriyoruz
		        elapsedTime=endTime-startTime;//aradaki farklý alýp bir deðiþkene atýyoruz
		        cout<<"Paralel Programlama Suresi: "<<elapsedTime*1000<<endl;//o deðiþkeni de ilgili formatta yazdýrýyoruz.
	    
		
	        char ciktiDosyasi[28];//Filtrelenmiþ matrisin çýktý halini char dizisi þeklinde tanýmladýk örn:GoruntuMatrisi1
    	    string ciktiDosyasiUzantisi= "_filtered.txt";//argümanýn sonuna uzantý eklemek için deðiþken tanýmladýk
    	    for(int s1=0; s1<15; s1++)//0 dan 15 e kadar gidicek for 
	        {
	          ciktiDosyasi[s1]=argv[1][s1+22];// argümanýn toplam (.txt hariç) 38 karakteri var biz sadece GoruntuMatrisi1 örneði þeklinde 15 karakterli bir deðer atadýk	
	        }
	        for(int s1=0; s1<14; s1++)//sonra o 15 deðerli karakterin sonuna 15 ten baþlayarak(15 ten alttaki forda baþlýyor) (devamý altta ki açýklama satýrýnda)
	        {
	          ciktiDosyasi[s1+15] = ciktiDosyasiUzantisi[s1];//uzantýyý atadýk artýk içinde GoruntuMatrisi1_filtered.txt örneði þeklinde bir deðer olmuþ oldu
	        } 								
		
	        ofstream yazilacakDosya;//yazilacak dosyayý çýktý dosyasý olarak ayarladýk
	        yazilacakDosya.open(ciktiDosyasi, ios::out);//burda char dizisini çevirme iþlemi ve yazýlacak dosyayý açma iþlemi yapýyoruz
            //yazilacakDosya<<satirlar;
            //yazilacakDosya<<sutunlar;
	        for(int s1=0; s1<satirlar; s1++)//satýrlara kadar giden bir döngü
            {
               for(int s2=0; s2<sutunlar; s2++)//sütunlara kadar giden bir döngü
	           {
		           yazilacakDosya<<yedekGoruntuMatrisiPointer[s1*sutunlar+s2]<<"\t";//yazýlacak matris deðerleri matrisin ilk satýr ve ilk sütunundan baþlýcak
                  //dikkat et matrisin satýr ve sütun bilgisinin yazdýðý satýrý almýcak onu almak isteseydik bu satýrýn üstündeki 7.ve 8.sütunu
                  //satý ve sütun sayýsý bilgisini yazdýrdýðýmýz kýsmý açýklama satýrýndan kaldýrmamýz gerekirdi
					
	           }
	           yazilacakDosya<<endl;//matris formatýnda yazdýrmak için her satýrdan sonra bir alta iniyoruz.
	        }	

	    yazilacakDosya.close();//yazýlacak dosyayý kapattýk 
	    delete [] yedekGoruntuMatrisiPointer;//diðer bilgisayarlara veri göndermek için kullandýðýmýz dinamikdiziyide sildik.
	    delete [] goruntuMatrisiPointer;//dinamnik diziyi sildik(filtreleme yaptýðýmýz)
	   
	}
	
	    delete [] lokalDiziPointer;//dinamik dizimizi silliyoruz
	    delete [] lokalDiziPointer2;//dinamik dizimizi siliyoruz
	  
	    
        MPI_Finalize();//MPI sonlandýrýyoruz
    return 0;//programý sonlandýrýyoruz
}
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  

