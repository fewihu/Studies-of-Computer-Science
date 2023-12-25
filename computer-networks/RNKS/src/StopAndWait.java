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



class StopAndWait{
	//debug
	private PrintWriter logWriter;	        //logging writer
	private boolean log;			//set by upper layer
	private int packets;			//number sended/received packets
	
	private long fileLength;
	private String fileName;
	
	private DatagramSocket socket;
	private InetAddress ia;
	private int destPort;
	private int ownPort;
	
	private packet Packet;			//instance of packet layer
	
	private byte[] toSend;			//data which should be send next
	
	private long	rto; 			//Retransmission Timeout
	private long	eRTT;			//estimated Timeout
	private long	sampleRTT;		//latency sample
	private double	weight;			//alpha to weight eRTT and sRTT
	private long	devRTT;			//eRTT variance
	
	private long 	time1;			//transmission start time
	private long	time2;			//transmission end time
	
	private int receivedFileCRC;	//File CRC in last packet
	private boolean lastPacketOK;	//correct SNr and PNr in last Packet
	
	private double	lossRate;		//simulate packet loss with lossRate probability
	private int		delay;			//simulate delay (in ms)
	private Random random;			//pseudorandom simulator
	
	private int		lostPackets;
	private int 	totalSleep;
	
	//constructor for client
	//@params: filelength, filename, destination, destination port, own port
	public StopAndWait(long fl, String fn, String dest, int Dport, int Oport){
		delay		= 0;
		lossRate	= 0.0;
		random		= new Random();
		lostPackets	= 0;
		totalSleep 	= 0;
		
		lastPacketOK = false;
		
		eRTT	= 1000;
		weight	= 0.125;
		devRTT	=  500;
		rto = eRTT + 4 * devRTT;
		
		Packet = new packet();
		
		//setting up logfile
		//log = true;
		//try{
		//	logWriter = new PrintWriter(new FileWriter("sw.log", true), true);
		//}catch(IOException ioe){
		//		System.out.println("ERROR: Logfehler");
                //            ioe.printStackTrace();
		//}
		
				
		//try to resolve destination address
		try{
			ia = InetAddress.getByName(dest);
		}catch(UnknownHostException e){
			System.out.println("ERROR: Unbekannte Zieladdresse");
			System.exit(-1);
		}
				
		//sanity check destination port number
		if(Dport >= 1024 && Dport <= 65535){
			this.destPort = Dport;
		}else{
			System.out.println("ERROR: ungültige Zielportnummer (1025 bis 65535)");
			System.exit(-1);
		}
		
		//sanity check own port number
		if(Oport >= 1024 && Oport <= 65535){
			this.ownPort = Oport;
		}else{
			System.out.println("ERROR: ungültige Empfangsportnummer (1025 bis 65535)");
			System.exit(-1);
		}
		
		//try to set up socket 
		try{
			socket = new DatagramSocket(ownPort);
		}catch(SocketException se){
			System.out.println("ERROR: Socket-Fehler");
			System.exit(-1);
		}
		
		//set length and name of file
		fileLength	= fl;
		fileName	= fn;
	}
	
	//constructor for server
	//@params: own port
	public StopAndWait(int Oport){
		lossRate	= 0.0;
		delay		= 0;
		random		= new Random();
		lostPackets = 0;
		totalSleep	= 0;
		
		lastPacketOK = false;
	
		eRTT	= 1000;
		weight	= 0.125;
		devRTT	=  500;
		rto = eRTT + 4 * devRTT;
		
		Packet = new packet();
		
		//setting up logfile
		//log = true;
		//try{
		//	logWriter = new PrintWriter(new FileWriter("sw.log", true), true);
		//}catch(IOException ioe){
		//		System.out.println("ERROR: Logfehler");
		//		ioe.printStackTrace();
		//}
		
		//sanity check own port number
		if(Oport >= 1024 && Oport <= 65535){
			this.ownPort = Oport;
		}else{
			System.out.println("ERROR: ungültige Empfangsportnummer (0 bis 65535)");
			System.exit(-1);
		}
		
		//try to set up socket 
		try{
			socket = new DatagramSocket(ownPort);
		}catch(SocketException se){
			System.out.println("ERROR: Socket-Fehler");
			System.exit(-1);
		}
	}
	
	public void setLossRate(double lr, int del){
		lossRate = lr;
		delay = del;
	}
	
	public void setLog(boolean x){
		log = x;
	}
	
	//delay thread and decide to throw away packet
	private boolean simulateWANisOK(){
		try{
			//Simulate network delay.
			int dur = (int) (random.nextDouble() * 2 * delay);
			totalSleep += dur;
			Thread.sleep(dur);
			//Decide whether to reply, or simulate packet loss
			return !(random.nextDouble() < lossRate);
		}catch(InterruptedException ie){
			System.out.println("ERROR: Interupt in sleep");
			System.exit(-1);
			return false;
		}
	}

	
	//initial fuction for server
	//receiveStartPacket -> set up server (session number, destination Port, destination ip) + send ack
	//@return: received filelength (bytes)
	public long receiveStartSendACK(){
		byte[] startData;
		byte[] byteACK	= new byte[3];
		
		DatagramPacket startPacket	= new DatagramPacket(new byte[1400], 1400);
		try{
			socket.receive(startPacket);
		}catch(IOException ioea){
			System.out.println("ERROR: Socket-Fehler");
			System.exit(-1);
		}
		startData	= startPacket.getData();
		destPort	= startPacket.getPort();
		ia			= startPacket.getAddress();
		if(Packet.checkStartPacket(startData)){	
			packets = 1;
			fileName	= Packet.getFileName();
			fileLength	= Packet.getFileLength();
			byteACK						= Packet.bindACK();
			DatagramPacket ACK			= new DatagramPacket(byteACK, byteACK.length, ia, destPort);
			try{
				time1 = System.currentTimeMillis();
				socket.send(ACK);
			}catch(IOException ioeb){
				System.out.println("ERORR: Socket-Fehler");
				System.exit(-1);
			}
			
			if(log){
				logWriter.println("----- START LOG -----");
				logWriter.println("received start packet, got data");
				logWriter.println("sended ACK");
			}
			return fileLength;
		}else{
			if(log){
				logWriter.println("----- START LOG -----");
				logWriter.println("received start packet");
				logWriter.println("something in packet is wrong, could not get data");	
			}
			return 0;
		}
	}
	
	//function for client
	//bind last packet (data) and set ready to send byte[]
	public void setDataOther(byte[] data){
		//packet request (data)
		toSend = Packet.bindDataPacket(data);
		
		//writing logfile
		if(log){
			logWriter.println("----------");
			logWriter.println("data packet binded");
			logWriter.println("data packet ready to send (SW-Layer)");
		}
	}
	
	//function for client
	//bind last packet (data + file CRC) and set ready to send byte[]
	public void setDataLast(byte[] data, long CRC){
		//packet request (data)
		toSend = Packet.bindLastDataPacket(data, CRC);
		
		//writing logfile
		if(log){
			logWriter.println("----------");
			logWriter.println("last data packet binded");
			logWriter.println("last data packet ready to send (SW-Layer)");
		}
	}
	
	//function for client
	//bind start packet and set ready to send byte[]
	public void setStart(){	
		//packet request (start)
		toSend = Packet.bindStartPacket(fileLength, fileName);
		
		//writing logfile
		if(log){
			logWriter.println("-----START LOG-----");
			logWriter.println("start packet binded");
			logWriter.println("start packet ready to send (SW-Layer)");
		}
	}
	
	//main function for client
	//send ready data packet + receive and check incoming ACk
	//obay rto and number trys
	//measure transmission time (time1 and time2)
	public boolean sendDataReceiveACK(){	
		
		byte[] ACK = new byte[3];
		
                System.out.println("dieses Packet versendet " + byteArrayToShort(toSend[0], toSend[1]) + " " + toSend[3]);
		
		DatagramPacket actPacket 	= new DatagramPacket(toSend,toSend.length,ia,destPort);
		DatagramPacket actACK		= new DatagramPacket(ACK, ACK.length);
		
		try{
			int i = 0;
			time1 = System.currentTimeMillis();
			socket.send(actPacket);

			socket.setSoTimeout((int)rto);
			//socket.setSoTimeout(3000);
			if(log){
				logWriter.println("----------");
				logWriter.println("packet send");
			}
			
			boolean packetLoss = false;
			while(true){
			

				try{
					if(packetLoss || this.simulateWANisOK()){
						socket.receive(actACK);
						if(Packet.checkACK(actACK.getData())){
							packets++;
							if(log){
								logWriter.println("ACK rceived and ok");
							}
							if(i == 0){
								time2 = System.currentTimeMillis();
							}else{
								time2 = -1;
							}
							break;
						}else{
                                                        System.out.println("ACK mit falscher ACK-Nummer oder falscher Sessionnummer erhalten");
							i++;
							socket.send(actPacket);
                                                        socket.setSoTimeout((int)rto);
                                                        continue;
						}
					}else{
						i++;
						lostPackets ++;
						packetLoss = true;
						continue;
					}

				}catch(SocketTimeoutException stoe){
                                        socket.send(actPacket);
                                        socket.setSoTimeout((int)rto);
					i++;
					System.out.println("Timeout " + i + "letztes Datenpacket wiederholt");
					if(i > 10){
						System.out.println("Zu viele Timeouts.");
						if(log){
							logWriter.println("----------");
							logWriter.println("to many timeouts, RTO: " + rto);
						}
						System.exit(-1);
					}
					System.out.println("Timeout");
					if(log){
						logWriter.println("----------");
						logWriter.println("Timeout " + i + " RTO: " + rto);
					}
					continue;
				}
			}
		}catch(IOException b){
			System.out.println("ERROR: Sende-Fehler");
			System.exit(-1);
		}
		return true;
	}
	
	//main function for server
	//receive data packet, which is checked by packet object
	//bind and send ACK
	//measure transmission time (time1 and time2)
	public byte[] receiveDataSendACK(){
		byte[] byteACK				= new byte[3];
		byte[] byteBuf				= new byte[1500];
		DatagramPacket actPacket 	= new DatagramPacket(byteBuf, 1500);
		int dataLength = 1;
		
		try{
			int i = 0;
			boolean packetLoss = false;
			while(true){

				try{
					socket.setSoTimeout((int)rto);
					//socket.setSoTimeout(3000);
					if(packetLoss || this.simulateWANisOK()){
						socket.receive(actPacket);
					}else{
						i++;
						lostPackets++;
						packetLoss = true;
						continue;
					}
					time2 = System.currentTimeMillis();
					dataLength = actPacket.getLength();
					byte[] packetBuf			= new byte[dataLength];
					packetBuf = actPacket.getData();
					if(Packet.checkDataPacket(packetBuf[0], packetBuf[1], packetBuf[2])){
						time2 = System.currentTimeMillis();
						packets++;                                        socket.setSoTimeout((int)rto);
						byteACK = Packet.bindACK();
						DatagramPacket actACK		= new DatagramPacket(byteACK, byteACK.length, ia, destPort);
						try{
							time1 = System.currentTimeMillis();
							this.calcRTO();
							socket.send(actACK);
							
							System.out.println("dieses ACK versendet" + byteArrayToShort(byteACK[0], byteACK[1]) + " " + byteACK[2]);
							
							byte[] ret = new byte[dataLength -3];
							for(int j=0; j < (dataLength - 3); j++){
								ret[j] = packetBuf[j+3];
							}
							
							if(log){
								logWriter.println("----------");
								logWriter.println("received data + checked data");
								logWriter.println("sended ACK");
							}
							
							return ret;


						}catch(IOException ioe){
							System.out.println("ERROR: Socket-Fehler");
						}
						break;
					}else{
                                            System.out.println("Datenpacket mit falscher ACK-Nummer oder falscher Sessionnummer erhalten");
                                            i++;
                                            byte[] latestACK = new byte[3];
                                            latestACK = Packet.latest();
                                            System.out.println("Dieses ACK wiederholt" + byteArrayToShort(latestACK[0], latestACK[1]) + " " + latestACK[2]);
                                            DatagramPacket latestPacket = new DatagramPacket(latestACK, 3, ia, destPort);
                                            socket.send(latestPacket);
					}
				}catch(SocketTimeoutException stoe){
					i++;
					byte[] latestACK = new byte[3];
					latestACK = Packet.latest();
                                        System.out.println("Dieses ACK wiederholt" + byteArrayToShort(latestACK[0], latestACK[1]) + " " + latestACK[2]);
					DatagramPacket latestPacket = new DatagramPacket(latestACK, 3, ia, destPort);
					socket.send(latestPacket);
					System.out.println("Timeout " + i + " letztes ACK wiederholt");
					if(i >= 10){
						System.out.println("Zu viele Timeouts.");
						System.exit(-1);
					}
				}
			}
		}catch(IOException ioe){
			System.out.println("ERROR: Socket-Fehler");
			System.exit(-1);
		}
		
		byte[] dummy = new byte[1];
		return dummy;
	
	}
		
	public void showRealLossRate(){
		if(lossRate > 0 || delay > 0){
			double rlr =  ((double)lostPackets)/((double)packets);
			DecimalFormat form = new DecimalFormat("#0.00");
			System.out.println("verlorene Packete: " + lostPackets + ", alle Packete: " + packets);
			System.out.println("reale Verlustrate: " + form.format(rlr));
			double rdel = ((double)totalSleep / (double)packets);
			System.out.println("reale Verzögerung: " + form.format(rdel) + " ms");
		}
	}
		
	//calculates new rto from time1 and time2 as sample 
	//time2 == -1 means transmission fail, sample should not be used
	//member rto is set 
	//eRTT and devRTT less then 50 are not allowed
	public void calcRTO(){
		if(time2 > -1){
			sampleRTT = time2 - time1;
			eRTT = ((long)((1 - weight) * eRTT)) + ((long)(weight * sampleRTT));
			devRTT = ((long)((1 - weight) * devRTT)) + ((long)(weight * (sampleRTT - eRTT)));
		}
		
		if(eRTT < 50)
			eRTT = 50;
		if(devRTT < 50)
			devRTT = 50;
		rto = eRTT + 4 * devRTT;
		
		if(log){
			logWriter.println("----------");
			logWriter.println("calculated new RTO");
			logWriter.println("sample: "	+ sampleRTT);
			logWriter.println("new rto: "	+ rto);
		}
	}
	
	public int getPacketsNum(){
		return packets;
	}
	
	public byte[] receiveLast(){
		byte[] byteACK				= new byte[3];
		byte[] bytePacket			= new byte[1500];
		DatagramPacket actPacket 	= new DatagramPacket(bytePacket, bytePacket.length);
		
		try{
			try{
				socket.setSoTimeout((int)rto);
				socket.receive(actPacket);
				packets++;
			}catch(SocketException se){
				System.out.println("ERROR: Socket-Fehler (SocketException)");
				System.exit(-1);
			}
		}catch(IOException ioec){
			System.out.println("ERROR: Socket-Fehler (IOException)");
			System.exit(-1);
		}
		
		bytePacket = actPacket.getData();
		
		int dataLength = 1499;
		
		while(dataLength > 0){
			if(bytePacket[dataLength] != 0){
				dataLength++;
				break;
			}
			dataLength--;
		}
		
		
		receivedFileCRC = Packet.byteArrayToInt(bytePacket[dataLength-4], bytePacket[dataLength-3], bytePacket[dataLength-2], bytePacket[dataLength-1]);
		
		
		if(Packet.checkDataPacket(bytePacket[0], bytePacket[1], bytePacket[2])){
			lastPacketOK = true;
		}else{}
		
		byte[] ret = new byte[dataLength - 7];
		for(int j=0; j < (dataLength - 7); j++){
			ret[j] = bytePacket[j+3];
		}
		return ret;
	}
	
	//FileLayer -> SW-Layer: if received FileCRC == calculated FileCRC
	public boolean sendLastACK(){
		if(lastPacketOK){
		
			byte[] byteACK = new byte[3];
			byteACK = Packet.bindACK();
			DatagramPacket actPacket = new DatagramPacket(byteACK, byteACK.length, ia, destPort);
			
			try{
				socket.send(actPacket);
			}catch(IOException ioed){
				System.out.println("ERROR: Socket-Fehler");
				System.exit(-1);
			}
			
			DatagramPacket repeatedPacket = new DatagramPacket(new byte[1500], 1500);
			try{
				socket.setSoTimeout(2500);
			}catch(SocketException se){
				System.out.println("ERROR: Socket-Fehler (last ACK Server sw)");
				System.exit(-1);
			}
			try{
				socket.receive(repeatedPacket);
			}catch(IOException ioee){}

			socket.close();
				
			//hier sollte Server warten und dann das Socket schließen
			
			return true;
		}else{
			return false;
		}
	}
	
	public int getReceivedCRC(){
		return receivedFileCRC;
	}
	
	public String getFileName(){
		return fileName;
	}
	
        public static int byteArrayToShort(byte a, byte b){
		int value = ((0xFF & a) << 8) | ((0xFF & b));
		return value & 0xffff;
	}

	//quasi file-layer
	//args: filename, 	s/r, 	l/n, destination, destination port
	//					send	log
	public static void main(String args[]){
	}
}

