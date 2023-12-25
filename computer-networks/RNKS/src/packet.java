import java.io.*;
import java.text.*;
import java.util.*;
import java.lang.*;
import java.nio.ByteBuffer;
import java.util.zip.CRC32;
import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;

class packet{

	private int		sessionNum;
	private int		packetNum;
	private CRC32	check;
	private Random	randomSN;
	private String	fn;
	private long	fl;
	
	
	//constructor (if object is server sessionNum needs to be changed after start packet, this is done within checkStartPacket)
	public packet(){

		check		= new CRC32();
		randomSN	= new Random();
		sessionNum 	= (short) randomSN.nextInt(65535);
		packetNum 	= 0;

		sessionNum = randomSN.nextInt(65535);
		packetNum = 0;
	}
	
	public String getFileName(){
		return fn;
	}
	
	public long getFileLength(){
		return fl;
	}
	
	//bind data of start packet to byte[]
	public byte[] bindStartPacket(long fileLength, String fileName){
	
		//set members
		fn	= fileName;
		fl	= fileLength;
	
		short fileNameLength = (short) fileName.length();
		ByteBuffer bb = ByteBuffer.allocate(18 + fileNameLength);
		packetNum = 0;
		String identifier = "Start";
		byte relativePN = (byte) (packetNum % 2);
		bb.putShort((short)sessionNum);
		bb.put(relativePN);
		bb.put(identifier.getBytes());
		bb.putLong(fileLength);
		bb.putShort(fileNameLength);
		bb.put(fileName.getBytes());
		check.update(bb.array());
		int CRCvalue = (int) check.getValue();
		ByteBuffer out = ByteBuffer.allocate(22 + fileNameLength);
		out.putShort((short)sessionNum);
		out.put(relativePN);
		out.put(identifier.getBytes());
		out.putLong(fileLength);
		out.putShort(fileNameLength);
		out.put(fileName.getBytes());
		out.putInt(CRCvalue);
		return out.array();
	}
	
	//calc CRC for startpacket data
	public int generateCRCstart(byte[] data){
		ByteBuffer buf = ByteBuffer.allocate(data.length);
		buf.wrap(data);
		check.update(buf.array());
		return (int) check.getValue();
	}
		
	//check identifier "Start" 
	public boolean checkBSI(byte[] startIdentifier){
		if (Arrays.equals("Start".getBytes(StandardCharsets.US_ASCII), startIdentifier))
			return true;
		else
			return false;
		
	}

	//compare PN in last packet with next PN expected
	public boolean checkPN(byte receivedPN){
        if(receivedPN == packetNum % 2){
			packetNum++;
            return true;
		}else
            return false;
	}
	
	//ceck for CRC and packetNum + set own SessionNum to receivedSN (Receiver Init)
	public boolean checkStartPacket(byte [] packetData){
		try{
		
			sessionNum 	= byteArrayToShort(packetData[0], packetData[1]) & 0xffff;
			fl			= byteArrayToLong(packetData[8], packetData[9], packetData[10], packetData[11], packetData[12], packetData[13], packetData[14], packetData[15]) & 0xffffffff;
			
			short fnl	= (short) byteArrayToShort(packetData[16], packetData[17]);
			byte[] fnb	= new byte[fnl];
			for(int i = 0; i < fnl; i++){
				fnb[i]	= packetData[i+18]; 
			}
			fn = new String(fnb);
			
			int length = 0;
			for(int j = 1023; j > 0; j--){
				if(packetData[j] != 0){
					length = j - 3;
					break;
				}
			}
			
			byte[] data = new byte[length];
			for(int i = 0; i < length; i++){
				data[i] = packetData[i];
			}
			check.update(data);
			int calcCRC = (int) check.getValue();
			byte[] b = new byte[4];
			for(int i = 0; i < 4; i++){
				b[i] = packetData[length + i];
			}
			
			int receivedCRC = byteArrayToInt(packetData[length], packetData[length + 1], packetData[length + 2], packetData[length + 3]);
			
			if(calcCRC == receivedCRC){
				
				return true;
			}
			else 
				return false;
		}catch(Exception e){
				System.out.println("ERROR: checkStartPacket (PacketLayer)");
				return false;
		}
		

	}
	
	public byte[] latest(){
            ByteBuffer bb = ByteBuffer.allocate(3);
            bb.putShort((short)sessionNum);
            bb.put((byte)((packetNum-1) % 2));
            return bb.array();
	}
	
	public byte[] bindACK(){
		ByteBuffer bb = ByteBuffer.allocate(3);
		bb.putShort((short)sessionNum);
		bb.put((byte)(packetNum % 2));
		packetNum++;
		return bb.array();
	}
	
	public boolean checkACK(byte[] data){
		int receivedSN	= byteArrayToShort(data[0], data[1]);
		int receivedACK = (int) data[2] & 0xff;
		if(receivedSN == sessionNum && receivedACK == (packetNum % 2)){
            packetNum++;
			return true;
        }
		else
			return false;
	}
	
	public byte[] bindDataPacket(byte[] data){
		ByteBuffer bb = ByteBuffer.allocate(1400 + 3);
		bb.putShort((short) sessionNum);
		byte relativePN = (byte) (packetNum % 2);
		bb.put(relativePN);
		bb.put(data);
		return bb.array();
	}
	
	public byte[] bindLastDataPacket(byte[] data, long CRCvalue){
		ByteBuffer bb = ByteBuffer.allocate(data.length + 3 + 4);
		bb.putShort((short) sessionNum);
		byte relativePN = (byte) (packetNum % 2);
		bb.put(relativePN);
		bb.put(data);
		bb.putInt( (int) CRCvalue);
		return bb.array();
	}
	
	
	//? sollte eigentlich SNr und PNr überprüfen
	public boolean checkDataPacket(byte a, byte b, byte c){
		int receivedSNr	= (int) byteArrayToShort(a, b);
		int receivedPNr	=  (int) c;
		if((receivedSNr == sessionNum) && (receivedPNr == (packetNum % 2))){
			return true;
		}else{}	
		return false;
	}
	
	public static int byteArrayToInt(byte a, byte b, byte c, byte d){
		int value = ((0xFF & a) << 24) | ((0xFF & b) << 16) |
            ((0xFF & c) << 8) | (0xFF & d);
		return value;
	}
	
	public static int byteArrayToShort(byte a, byte b){
		int value = ((0xFF & a) << 8) | ((0xFF & b));
		return value & 0xffff;
	}
	
	public static long byteArrayToLong(byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h){
		long value	= 	((0xFF & a) << 56) | ((0xFF & b ) << 48) | ((0xFF & c) << 40) | ((0xFF & d) << 32) | ((0xFF & e) << 24) | ((0xFF & f) << 16) | 				((0xFF & g) << 8 ) | (0xFF & h);
		return value;
	}
			
	public static void main(String args[]){
		
	}
}
