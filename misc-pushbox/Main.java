import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class Main {
    public static final char WALL = '8';
    public static final char HUMAN = '4';
    public static final char DESTNATION = '1';
    public static final char BOX = '2';
    public static final char SPACE = '0';
    public static final int MAXMUM = 20000000;
    public static boolean[] status;
    public static int[] humanPositionX;
    public static int[] humanPositionY;
    public static int[][] box_PositionX;
    public static int[][] box_PositionY;
    public static int[] destinationX;
    public static int[] destinationY;


    public  static int iLength;
    public static int jLength;
    public static int[] pow;
    public static int[] direction;
    public static int[] last;
    public static  final int[] DX = {-1,1,0,0};//上下左右
    public static final int[] DY = {0,0,-1,1};

    public static  int boxNum;
    public static int count = 0;
    public static String ans;
    public static void main(String[] args) throws Exception{
        long begin = System.currentTimeMillis();
        Socket socket = new Socket("34.92.121.149",9091);
        InputStream inputStream = socket.getInputStream();
        OutputStream outputStream = socket.getOutputStream();
        Scanner scanner = new Scanner(inputStream);
        PrintWriter printWriter = new PrintWriter(outputStream);
        scanner.nextLine();
        scanner.nextLine();
       // System.out.println(scanner.nextLine());
        //System.out.println(scanner.nextLine());

        for (int j = 0; j <25;j++){
          //  System.out.println(j);
            String mapStr = "";
            ans = "";
            String s = scanner.nextLine();
            if (s.length() == 0)
                s = scanner.nextLine();
           // System.out.println(s);
            while (s.charAt(0) != 't'){

                mapStr = mapStr + s+"\n";
                s = scanner.nextLine();
              //  System.out.println(s);
            }

            String[] a = mapStr.split("\n");
            char[][] map = new char[a.length][a[0].length()];
            for(int i = 0; i < a.length; i++){
                map[i] = a[i].toCharArray();
            }

            initial(map);
            BFS(map);
           // System.out.println(ans);
//            s = scanner.nextLine();
          //  System.out.println(ans.length());

           printWriter.write(ans+"\n");
           printWriter.flush();
        //   System.out.println("已发送");
        }
        String s = scanner.nextLine();
        System.out.println(s);
        long end = System.currentTimeMillis();
        System.out.println(end - begin);
//        String mapStr =
//                "8888888888\n" +
//                        "8888880088\n" +
//                        "8880200088\n" +
//                        "8880018088\n" +
//                        "8888081008\n" +
//                        "8880400208\n" +
//                        "8880008888\n" +
//                        "8888888888";



    }
    public static void initial(char[][] map){
        iLength = map.length;
        jLength = map[0].length;
        boxNum = 0;
        for (int i = 0;i<iLength;i++)
            for (int j = 0;j<jLength;j++)
                if (map[i][j] == BOX)
                    boxNum++;
    }
    public static void BFS(char[][] map){
       // len = map.length;
        pow = new int[5];
        pow[0] = 1;
        pow[1] = (iLength-2)*(jLength-2);
//        pow[2] = pow[1] * pow[1];
//        pow[3] = pow[2] * pow[2];
        for (int i = 2 ; i < 5;i++){
            pow[i] = pow[i-1] * pow[1];
        }
        //箱子的位置
        box_PositionX = new int[boxNum][MAXMUM];
        box_PositionY = new int[boxNum][MAXMUM];

        //目的地位置
        destinationX = new int[boxNum];
        destinationY = new int[boxNum];

        //人的位置
        humanPositionX = new int[MAXMUM];
        humanPositionY = new int[MAXMUM];

        //保存行走的路径
        last = new int[MAXMUM];
        direction = new int[MAXMUM];




        int temp = 0;
        int tempDes = 0;

        for (int i = 0; i < iLength;i++){
            for (int j = 0;j < jLength;j++){
                if (map[i][j] == HUMAN){
                    humanPositionX[0] = i;
                    humanPositionY[0] = j;
                    // map[i][j] = SPACE;
                }
                if (map[i][j] == BOX){
                    box_PositionX[temp][0] = i;
                    box_PositionY[temp][0] = j;
                    //map[i][j] = SPACE;
                    temp++;
                }
                if (map[i][j] == DESTNATION){
                    destinationX[tempDes] = i;
                    destinationY[tempDes] = j;
                    tempDes++;
                }

            }
        }

        //状态数组，确保状态不重复
        status = new boolean[MAXMUM];

        //箱子的临时位置
        int[] tempBoxX = new int[boxNum];
        int[] tempBoxY = new int[boxNum];

        for (int i = 0; i< boxNum; i++){
            tempBoxX[i] = box_PositionX[i][0];
            tempBoxY[i] = box_PositionY[i][0];
        }
//        tempBoxX[0] = box_PositionX[0][0];
//        tempBoxX[1] = box_PositionX[1][0];
//       // tempBoxX[2] = box_PositionX[2][0];
//
//        tempBoxY[0] = box_PositionY[0][0];
//        tempBoxY[1] = box_PositionY[1][0];
//        //tempBoxY[2] = box_PositionY[2][0];


        //计算初始状态
        int initialStatus = 0;
        initialStatus = computeStatus(tempBoxX,tempBoxY,humanPositionX[0],humanPositionY[0]);


        status[initialStatus] = true;
        int head = 0;
        direction[head] = -1;
        last[head] = -1;
        int tail = 1;

        while (head < tail){

            //  System.out.println(head);

            //检查是否结束
            if (checkOver(head) == true){
              //  System.out.println("win");
                printPath(head);
                break;
            }

            //四个行走方向
            for ( int i = 0 ;i < 4 ; i ++){
                int peopleX = humanPositionX[head] + DX[i];
                int peopleY = humanPositionY[head] + DY[i];

                //确保不越界
                if (peopleX<0 || peopleY <0 || peopleX>= iLength || peopleY >= jLength)
                    continue;

                //排除墙
                if (map[peopleX][peopleY] == WALL){
                    continue;
                }


                int tempStatus;

                for (int j = 0; j < boxNum; j++){
                    tempBoxX[j] = box_PositionX[j][head];
                    tempBoxY[j] = box_PositionY[j][head];
                }

                int k = -1;
                int tempx = -1;
                int tempy = -1;

                //走的地方不是箱子
                if (notBox(peopleX,peopleY,head)){
                    tempStatus = computeStatus(tempBoxX,tempBoxY,peopleX,peopleY);
                }
                else {
                    tempx = peopleX + DX[i];
                    tempy = peopleY + DY[i];
                    //箱子撞墙 或箱子
                    if (map[tempx][tempy] == WALL || !notBox(tempx,tempy,head))
                        continue;

                    for (int j = 0; j< boxNum; j++){
                        if (box_PositionX[j][head] == peopleX && box_PositionY[j][head] == peopleY){
                            k = j;
                            tempBoxX[j] = tempx;
                            tempBoxY[j] = tempy;

                        }
                    }
                }
                tempStatus = computeStatus(tempBoxX,tempBoxY,peopleX,peopleY);

                //存在状态不重复
                if (status[tempStatus])
                    continue;


                tail++;
                humanPositionX[tail] = peopleX;
                humanPositionY[tail] = peopleY;
                for (int j = 0; j < boxNum; j++){
                    box_PositionX[j][tail] = tempBoxX[j];
                    box_PositionY[j][tail] = tempBoxY[j];
                }
//                if (k != -1){
//                    map[peopleX][peopleY] = SPACE;
//                   // box_PositionX[k][tail] = tempx;
//                   // box_PositionY[k][tail] = tempy;
//                    map[tempx][tempy] = BOX;
//                }
                status[tempStatus] = true;
                direction[tail] = i;
                last[tail] = head;
            }

            head++;
        }

    }
    public static int  computeStatus(int[] tempBoxX,int[] tempBoxY ,int pX,int pY ){
        int status = 0;
        for (int i = 0; i< boxNum; i++){
            status = status + ((tempBoxX[i]-1)*(jLength-2) + (tempBoxY[i]-1))*pow[i];
        }
        status = status + ((pX-1)*(jLength-2) + (pY-1))*pow[boxNum];
        return status;

    }
    public static  boolean checkOver(int k){
        for (int i = 0; i< boxNum; i++){
            boolean flag = false;

            for(int j = 0; j < boxNum; j++)
                if (destinationX[i] == box_PositionX[j][k] && destinationY[i] == box_PositionY[j][k])
                    flag = true;

            if ( flag == false)
                return false;
        }
        return true;
    }

    public static void printPath(int k){
        if (last[k] != -1)
            printPath(last[k]);
     //   System.out.print(count++);
        if (direction[k] == 0){
            //System.out.print(" w" + " ");
            ans = ans + "w";
        }

        if (direction[k] == 1){
         //   System.out.print(" s" + " ");
            ans = ans + "s";
        }

        if (direction[k] == 2){
          //  System.out.print(" a" + " ");
            ans = ans + 'a';
        }

        if (direction[k] == 3){
         //   System.out.print(" d" + " ");
            ans = ans + 'd';
        }

       // System.out.println();
    }
    public static boolean notBox(int x, int y,int k){
        for (int i = 0; i< boxNum; i++)
        {
            if (box_PositionX[i][k] == x && box_PositionY[i][k] == y){
                return false;
            }
        }
        return true;
    }
}
