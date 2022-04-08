#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>          //file input
#include <vector>           //vector
#include <cmath>            //square

//function that gets thee maximum number of passenger
int maxPassenger(int row, int col, double seatW, double seatP);
//function that gets the radius from the number of passengers
double getRadius(int row, int col, double seatW, double seatP, int numPeople);
//function that finds the x corrdinates of seats
double **storeSeatsX(int row, int column, double seatW, double seatP, double radius);
//function that finds the y corrdinates of seats
double **storeSeatsY(int row, int column, double seatW, double seatP, double radius);
//function that stores the locations of the passengers in 2 arrays of vector
void storeLocations(std::vector<int> row[], std::vector<int> column[], double **x, double **y, double radius, int numOfRow, int numOfColumn);
//function that checks if two circles intersect (if two passengers are too close to each other)
bool circleIntersect(double x1, double y1, double x2, double y2, double rad);
//function that coverts the seats from rows and columns into seat numbers
void storeSeat(std::string seatList[],std::vector<int> row[], std::vector<int> column[], int people, int numOfColumn);

int main() {
    std::string airplaneName, line;              //name of the user //each lines
    std::string inputFileName, outputFileName;   //filename
    std::ifstream file;                          //input file
    std::ofstream outputFile;                    //output file
    std::vector<std::string> names;              //airplanes name and customer names
    
    //open files
    inputFileName = "/Users/jasonsung/desktop/cs113/airplane_seats/airplane_seats/plane.txt";
    file.open(inputFileName);
    outputFileName ="/Users/jasonsung/desktop/cs113/airplane_seats/airplane_seats/result.txt";
    outputFile.open(outputFileName);
    
    //store every line into the vector
    while (getline(file,line))
    {
        names.push_back(line);
    }
    file.close();
    
    //check what airplane it is
    if (names.at(0)=="Fastplane123")
    {
        //information of Fastplane123
        int numOfRows = 6;          //the number of rows for Fastplane 123
        int numOfColumn = 7;        //the number of columns for Fastplane 123
        double seatWidth = 3;       //seeat width
        double seatPitch = 4;       //the distance from any point on one seat to the exact same point on the seat in front or behind it
        int numOfPeople;            //number of customers
        

        numOfPeople = names.size()-1;    //minus one because the first index is the airplane name
        std::string seat[numOfPeople];   //string array that stores the seats for output text file
        
        //check if there are too many passengers to fit into the plane
        if (numOfPeople > maxPassenger(numOfRows, numOfColumn, seatWidth, seatPitch))
        {
            outputFile << "exceed maximum number of passengers for this airplane...\n";
            
        }
        else{
            //get the radius
            double radius = getRadius(numOfRows, numOfColumn, seatWidth, seatPitch, numOfPeople);
            double **seatsX, **seatsY;  //two arrays of vectors that store the x and y corrdinate of each seat (the index is row# and column#)
            std::vector<int>row[numOfColumn];
            std::vector<int>colum[numOfColumn];
            
            seatsX = storeSeatsX(numOfRows, numOfColumn, seatWidth, seatPitch, radius); //stores the x coordinate
            seatsY = storeSeatsY(numOfRows, numOfColumn, seatWidth, seatPitch, radius); //stores the y coordinate
          
            // DEBUG: output all x-y coordinates of the seats
            for(int i=0; i<numOfRows; i++)
            {
                for(int q = 0 ; q<numOfColumn; q++)
                {
                    std::cout<<"("<<seatsX[i][q]<<", "<<seatsY[i][q]<<") ";
                }
                std::cout<<std::endl;
            }
            
            
            //store locations as rows and columns
            storeLocations(row, colum, seatsX, seatsY, radius, numOfRows, numOfColumn );
            //store seats as seat number
            storeSeat(seat, row, colum, numOfPeople, numOfColumn);
            
            // DEBUG: output the "good" seats in terms of eat numbers
            for (int a = 0; a<numOfPeople; a++){
                std::cout<<seat[a]<<std::endl;
            }
            
            //output to text file
            for (int a = 0; a<numOfPeople; a++){
                 outputFile << names.at(a+1)<<": " <<seat[a]<<"\n";
            }
        }
        
    }
   
    outputFile.close(); //close file
    std::cout << "program ends..." << std::endl;
    return 0;
}

//function that gets thee maximum number of passenger
int maxPassenger(int row, int col, double seatW, double seatP)
{
    double area;
    int maxPeople;
    //get the area of the plane
    area = seatP * row * seatW * (col + 2); //2 aisles
    //get the number of circles that can fit into the plane with radius "6"
    maxPeople = area/(3*3*M_PI);
    return maxPeople;
}

//function that gets the radius from the number of passengers
double getRadius(int row, int col, double seatW, double seatP, int numPeople)
{
    double area;
    double radius;
    //get the area of the plane
    area = seatP * row * seatW * (col + 2); //2 aisles
    radius = area/(numPeople*M_PI);         //get the radius by deviding the num of people and PI
    radius = sqrt(radius);
    return radius;
}

//function that finds the x corrdinates of seats
double **storeSeatsX(int row, int column, double seatW, double seatP, double radius)
{
    //----the origin is the bottom left corner----
    
    double x = seatW*(column+2);//+2 aisles, get how wide the airplane is  (maximum x)

    double **seat = 0;
    seat = new double *[row];       //create an array of pointers, size is the number of rows
 
    for (int r = 0; r<row; r++) //go throught each row
    {
        double dx = x-(seatW/2); // get the x coordinate of the seat by deviding the seat width by two, starting from the right
    
        seat[r] = new double[column];   //for each pointer in the array, create anoter array, size is the number of columns
       
        for(int c = column-1; c>=0; c--)  //go through each column
        {
            
            seat[r][c] = dx;    //store x
            
            if (c == 2||c==5)   //skip the two aisles
                dx -= (2*seatW);
            else
                dx -= seatW;    //one column to the left has an x-difference of the seat width
        }
        
    }
    
    
    return seat;
}

//function that finds the x corrdinates of seats
double **storeSeatsY(int row, int column, double seatW, double seatP, double radius)
{
    //----the origin is the bottom left corner----
    
    double y = seatP*row;   //+2 aisles, get how long the airplane is  (maximum y)
   
    double **seat = 0;
    seat = new double *[row];//create an array of pointers, size is the number of rows
 
    y -= (seatP/2); // get the first y coordinate of the seat by subtracting the max y by the half of the seat pitch, starting from the top
    
    for (int r = 0; r<row; r++)//go thru each row
    {
       
        seat[r] = new double[column];
        for(int c = 0; c<column; c++)   //go thru each column
        {
            seat[r][c] = y;    //every column has the same y
        }
         y -= seatP;        //go down one row
    }
    
    return seat;
}

//function that stores the locations of the passengers in 2 arrays of vector
void storeLocations(std::vector<int> row[], std::vector<int> column[], double **x, double **y, double radius,  int numOfRow, int numOfColumn)
{
    int dy= 0;  //first column y
    
    row[0].push_back(0);
    column[0].push_back(0);
  
    //store seats for first column
    for(int q =1 ; q<numOfRow; q++) //go thru each row in the first column
    {
        if(!circleIntersect(x[dy][0], y[dy][0], x[q][0], y[q][0], radius))//execute if two passengers are not too close to each other (circles dont intersect)
        {
            dy = q;
            column[0].push_back(0); //first column has a index of 0 for every row
            row[0].push_back(q);    //stores the rows for the first column
        }
    }

    std::cout<<std::endl;
   
    //stores every other columns
    for(int i=1; i<numOfColumn; i++)//for each column starting from the second column
    {
        int leftColumn = i-1;    //stores the left column
        
        while(row[leftColumn].size()==0)     //find the left column to compare to
        {
            leftColumn -= 1;        //if there's no "good" seats in this colomn, go to the one on the left
        }
        
        for(int k = 0; k<numOfRow ; k++)    //go thru each rows for each column
        {
            bool goodSpot = true;   //stores if the seat is a "good" seat
            
            for (int a = 0; a<row[leftColumn].size(); a++) //compare this seat with each "Good" seat on the left column
            {
                //if they intersect, it is not a "good" seat
                if(circleIntersect(x[row[leftColumn].at(a)][column[leftColumn].at(a)], y[row[leftColumn].at(a)][column[leftColumn].at(a)], x[k][i], y[k][i], radius))
                {
                    goodSpot = false;
                }
            }
                                                      
            if (goodSpot)   //if it does not interest with the left column, compare to the "good" seat above it
            {
                if (row[i].size()==0) //if there is no other people sitting in the same column above this seat, this seat is a "good" seat
                {
                    column[i].push_back(i);     //store its column
                    row[i].push_back(k);        //store its row
                }
                else{   //if there is another "good" seat above
                    int loc =row[i].size()-1;
                    //if this seat does not intersect witht the "good" above, this seat is also a "good" seat
                    if(!circleIntersect(x[row[i].at(loc)][column[i].at(loc)], y[row[i].at(loc)][column[i].at(loc)], x[k][i], y[k][i], radius))
                    {
                        column[i].push_back(i); //store its column
                        row[i].push_back(k);    //store its row
                    }
                }
            }
        }
    
    }
    //  DEBUG: output "good" seats in terms of rows and columns
    for (int a = 0; a<numOfColumn; a++)
    {
        for (int i =0; i< row[a].size(); i++)
            std::cout<<"row: "<<row[a].at(i)<<", column: "<<column[a].at(i)<<std::endl;
    }
}

//function that checks if two cricles intersects
bool circleIntersect(double x1, double y1, double x2, double y2, double rad)
{
    //get the square of the distance between the centers of two circles
    double sqOfDistance = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    //get the square of the radius sum
    double sqOfRadSum = (2*rad) * (2*rad);
    
    if(sqOfDistance > sqOfRadSum)//don't intersect
        return false;
    else                        //intersects
        return true;
}

//function that coverts the seats from rows and columns into seat numbers
void storeSeat(std::string seatList[],std::vector<int> row[], std::vector<int> column[], int people, int numOfColumn)
{
    std::string seatRow, seatColumn;
    int num, loc = 0;
    
    for (int i=0; i<numOfColumn; i++)   //go thru each column
    {
        for (int q=0; q<row[i].size();q++)  //go thru each row
        {
            seatList[loc]="";
            num = row[i].at(q)+1;
            seatRow = std::to_string(num); //convert to string
            seatList[loc].append(seatRow); //store the row number into each element of the array
            
            switch (column[i].at(q))    //for column, we have to comvert from number to characters
            {
                case 0:
                    seatColumn="A";
                    break;
                case 1:
                    seatColumn="B";
                    break;
                case 2:
                    seatColumn="C";
                    break;
                case 3:
                    seatColumn="D";
                    break;
                case 4:
                    seatColumn="E";
                    break;
                case 5:
                    seatColumn="F";
                    break;
                case 6:
                    seatColumn="G";
                    break;
            }
            seatList[loc].append(seatColumn); //strore the column number
            loc++;
        }
    }

}

