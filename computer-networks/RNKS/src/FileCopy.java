//Author: Felix Mueller 
//s79138 
//felix.mueller@htw-dresden.de

import java.io.*;
import java.text.*;
import java.util.*;
import java.lang.*;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.net.*;
import java.util.zip.CRC32;

class FileCopy{

	//hold filedata and metadata
	private ByteBuffer file;
	private String fileName;
	private long fl;

	//read/write file
	private FileOutputStream fos;
	private FileInputStream fis;
	private File f;

	//CRC
	private CRC32 check;
	private long fileCRC;
	
	private boolean isClient;
	private StopAndWait sw;

	//to calculate data rate
	private long time1;		//start of transmission
	private long time2;		//sample time
	private int timeDif;	//difference between start and sample aka time since start

	//to simulate network delay and packet loss
	private int	delay;
	private double lossRate;
	
	//constructor
	public FileCopy(String state){
		if(state.equals("s")){
			System.out.println();
			isClient = true;
			System.out.println("SENDER:");
		}else{
			isClient = false;
			System.out.println("SERVER:");
		}
	}
		
	public void setUpClient(String fn, String dest, String destPort, int delay, double lossRate){
	
	
		//TO-DO: dest unterscheidung ip/hostname 
		//sanity check
	
		fileName = fn;
		f = new File(fileName);
		fl = 0;
		check = new CRC32();

		try{
			fis = new FileInputStream(f);
			try{
				fl = fis.available();
				byte[] CRCbuffer = new byte[ (int) fl];
				fis.read(CRCbuffer);
				check.update(CRCbuffer);
				fileCRC = check.getValue();
				fis.close();
				fis = new FileInputStream(f);
			}catch(IOException ioe){
				System.out.println("ERROR: Datei: " + fileName + " konnte nicht gelesen werden.");
			}
		}catch(FileNotFoundException fnfe){
			System.out.println("ERROR: Es gibt keine Datei " + fileName);
		}
		
		//@args: fileLength, fileName, destination address, destination port, own port
		sw = new StopAndWait(fl, fileName, dest, Integer.parseInt(destPort), 4000);
		sw.setLossRate(lossRate, delay);
		sw.setLog(false);
	}
	
	public void sendFile(){
		//start packet
		sw.setStart();
		time1 = System.currentTimeMillis();
		time2 = System.currentTimeMillis();
		sw.sendDataReceiveACK();
		sw.calcRTO();
		
		try{
			//other packets
			byte[] data = new byte[1400];
			while(fis.available() > 1400){
				fis.read(data);
				sw.setDataOther(data);
				sw.sendDataReceiveACK();
				timeDif =  (int) (System.currentTimeMillis() - time2);
				if(timeDif >= 350){
					long BytesSend	= fl - fis.available();
					double dataRate	= (BytesSend*1000)/timeDif;
					if(dataRate > 550){
						dataRate = dataRate / 1024;
						DecimalFormat form = new DecimalFormat("#0.00");
						System.out.println("aktuelle Datenrate: " + form.format(dataRate) + " KB/s");
					}else{
						System.out.println("aktuelle Datenrate: " + dataRate + " B/s");
					}
					time2 = System.currentTimeMillis();
				}
				sw.calcRTO();
			}
				
			//last packet + fileCRC
			int remaining = fis.available();
			byte[] dataRemaining = new byte[remaining];
			fis.read(dataRemaining);
			long fileCRC = check.getValue();
				
			sw.setDataLast(dataRemaining, fileCRC);
			sw.sendDataReceiveACK();
			}catch(IOException ioe){
				System.out.println("ERROR: IOException FileCopy-Layer");
			}
			
			time2	= System.currentTimeMillis();
			int totalTimeDif = (int) (System.currentTimeMillis() - time1);
			double dataRate	= (fl*1000)/totalTimeDif;
			if(dataRate > 550){
				dataRate = dataRate / 1024;
				DecimalFormat form = new DecimalFormat("#00.00");
				System.out.println("mittlere Datenrate: " + form.format(dataRate) + " KB/s");
			}else{
				System.out.println("mittlere Datenrate: " + dataRate + " B/s");
			}
			
			sw.showRealLossRate();
	}
	
	public void setUpServer(String Oport, int delay, double lossRate){
		sw = new StopAndWait(Integer.parseInt(Oport));
		sw.setLog(false);
		sw.setLossRate(lossRate, delay);
		check = new CRC32();
	}

	public void receiveFile(){
		fl = sw.receiveStartSendACK();
		
		long fileLengthReadable = fl;
		int i = 0;
		while (fileLengthReadable > 550){
			fileLengthReadable /=1024;
			i++;
		}
		fileLengthReadable++;
		
		String[] prefix = new String[9];
		prefix[0] = "B";
		prefix[1] = "KiB";
		prefix[2] = "MiB";
		prefix[3] = "GiB";
		prefix[4] = "TiB";
		prefix[5] = "PiB";
		prefix[6] = "EiB";
		prefix[7] = "ZiB";
		prefix[8] = "YiB";
		
		String receivedFileName = sw.getFileName();
		System.out.println("Übertragungswunsch für die Datei: " + receivedFileName + "(" + fileLengthReadable + " " + prefix[i] + ")");
		int extLoc = receivedFileName.lastIndexOf(".");
		String extension = receivedFileName.substring(extLoc);
		String newFileName = receivedFileName.substring(0, extLoc);
		File f = new File(newFileName + extension);
		while (f.exists()) {
			newFileName += "1";
			f = new File(newFileName + extension);
		}
		
		file = ByteBuffer.allocate((int)fl);
	
		while(fl > 1400){
			file.put(sw.receiveDataSendACK());
			sw.calcRTO();
			fl -= 1400;
		}
		
		file.put(sw.receiveLast());

		check.update(file.array());
		fileCRC = (int)check.getValue();
		if(fileCRC == sw.getReceivedCRC()){
			try{
				fos = new FileOutputStream(f);
				fos.write(file.array());
				fos.close();
				System.out.println("Übertragung erfolgreich");
				System.out.println("Datei als " + newFileName + extension + " gespeichert");
			}catch(Exception e){
				System.out.println("ERROR: writing received data fail");
			}
			sw.sendLastACK();
		}
		
		sw.showRealLossRate();
	}
	
	//@args: send, fileName, destination, destination port
	//@args: receive, ownport
	public static void main(String[] args){
	
	
		FileCopy FC;
		if(args[0].equals("s")){
			FC = new FileCopy(args[0]);
			//@args: filename, dest, destport, delay, lossRate
			FC.setUpClient(args[1], args[2], args[3], Integer.parseInt(args[4]), Double.parseDouble(args[5]));
			FC.sendFile();
		}else if(args[0].equals("r")){
			FC = new FileCopy(args[0]);
			while(true){
				FC.setUpServer(args[1], Integer.parseInt(args[2]), Double.parseDouble(args[3]));
				//@args: own port, delay, lossRate
				FC.receiveFile();
			}
		}else{
			System.out.println("Parameterfehler");
			System.exit(-1);
		}
	}
}
