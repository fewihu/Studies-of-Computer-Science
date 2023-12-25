

class GF_LUT{


	static int[][] addTable =
	{{0,1,2,3,4,5,6,7},{1,0,3,2,5,4,7,6},{2,3,0,1,6,7,4,5},{3,2,1,0,7,6,5,4},
	 {4,5,6,7,0,1,2,3},{5,4,7,6,1,0,3,2},{6,7,4,5,2,3,0,1},{7,6,5,4,3,2,1,0}};
	static int[][] mulTable=
	{{0,0,0,0,0,0,0,0},{0,1,2,3,4,5,6,7},{0,2,4,6,3,1,7,5},{0,3,6,5,7,4,1,2},
	 {0,4,3,7,6,2,5,1},{0,5,1,4,2,7,3,6},{0,6,7,1,5,3,2,4},{0,7,5,2,1,6,4,3}};
	 
	static String[] polynoms=
	{"0", "1", "a", "a + 1", "a^2", "a^2 + 1", "a^2 + a", "a^2 + a + 1"};

	public static void main(String[] args){
		
		if(args.length != 3) {System.out.println("usage: java GF_LUT <power first operand> [*|+] <power second operand>"); System.exit(1);}
		
		switch(args[1]){
		
			case "*":
				System.out.println("result: " + polynoms	[mulTable[java.util.Arrays.binarySearch(polynoms, args[0])]
															[java.util.Arrays.binarySearch(polynoms, args[2])]] );
				System.exit(0);
			case "+":
				System.out.println("result: " + polynoms	[addTable[java.util.Arrays.binarySearch(polynoms, args[0])]
															[java.util.Arrays.binarySearch(polynoms, args[2])]]);
				System.exit(0);
			default:
				System.out.println("Operand not in [+|*]");	
				System.exit(1);
		}
	}
}
