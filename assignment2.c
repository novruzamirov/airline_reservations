#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//IN ORDER TO SHOW THE PLANE
void show_plane(int numOfRows, int rowNumArray[], int numOfAisles, int seatsPerRow, char plane[110][40]){
    for(int i = 0; i < numOfRows + 1; i++){
        if(i != 0){
            printf("%-3d", rowNumArray[i-1]);
        }
        else{
            printf("   ");
        }
        for(int j = 0; j < seatsPerRow + (numOfAisles); j++){
            printf("%c", plane[i][j]);
            if(plane[i][j] == '|'){
                printf("  |"
                       );
            }
            printf(" ");
        }
        printf("\n");
    }
}

//IN ORDER TO FIND THE MAX SCORE AMONG THE SCORES OF SEATS
double findMax(int numOfRows, int seatsPerRow, double** arr, int numOfReservedSeats){
    double max = arr[0][2]; // score
    for(int i = 0; i < (numOfRows * seatsPerRow) - numOfReservedSeats; i++){
        if(arr[i][2] > max){
            max = arr[i][2];
        }
    }
    
    return max;
}

//IN ORDER TO MEASURE DIST BETWEEN SEATS
double dist(double* s1, int* s2){
    double distance = 0;
    int s1Int1 = s1[0];
    int s2Int1 = s2[0];
    int s1Int2 = s1[1];
    int s2Int2 = s2[1];
    
    distance = (abs(s1Int1 - s2Int1)) + (abs(s1Int2 - s2Int2) / 2.0);
    
    return distance;
}

double adjusted_dist(double* s1, int* s2){
    double adjusted_distance = 0;
    
    if(dist(s1, s2) == 0){
        adjusted_distance = 0;
    }
    
    else{
        adjusted_distance = (100 - (pow(100, 1 / (dist(s1, s2) ) ) ) );
    }
    
    return adjusted_distance;
}

//IN ORDER TO CALCULATE THE SCORE OF EACH EMPTY SEAT
double score(double* s, char plane[110][40], int numOfRows, int numOfAisles, int seatsPerRow){
    double score = 0;
    
    for(int i = 1; i < numOfRows + 1; i++){
        for(int j = 0; j < seatsPerRow + numOfAisles; j++){
            if(plane[i][j] != '-' && plane[i][j] != '|'){
                int si[2] = {i, (j + 1)};
                score += ((int)(adjusted_dist(s, si)*100000))/100000.0;
            }
        }
    }
    
    return score;
}

//CALCULATING THE SCORES OF GROUP AND FINDING MAX GROUP SCORE
double seat_block_score(int peopleNum, char plane[110][40], int numOfRows, int numOfAisles, int seatsPerRow, int* aisles, double* g){
    double maxGroupScore = -9999999999;
    double groupScore = 0;
    int z = 0;
    int aisleCounter = 0;
    int maxAisleScore = 10;
    
    
    
    for(int i = 1; i < numOfRows+1; i++){
        z = 0;
        for(int j = 0; j < seatsPerRow+numOfAisles; j++){
            if(plane[i][j] == '-'){
                z++;
            }
            else if(plane[i][j] == '|'){
                continue;
            }
            else{
                z = 0;
                continue;
            }
            
            if(z >= peopleNum){
                groupScore = 0;
                aisleCounter = 0;
                int b = 1;
                int k = 0;
                while (b < peopleNum){
                    if(plane[i][j-1-k] == '|'){
                            aisleCounter++;
                    } else {
                        b++;
                    }
                    k++;
                }
                
                for(int k = 1; k <= peopleNum+aisleCounter; k++){
                    double s[2];
                    s[0]=i;
                    if (plane[i][j-peopleNum-aisleCounter+k] == '-') {
                        s[1]=j-peopleNum-aisleCounter+k+1;
                    } else {
                        continue;
                    }

                    groupScore += score(s, plane, numOfRows, numOfAisles, seatsPerRow);
                    
                }
                if(groupScore > maxGroupScore || (groupScore == maxGroupScore && aisleCounter < maxAisleScore) ){
                    maxGroupScore = groupScore;
                    g[0] = i;
                    g[1] = j;
                    g[2] = groupScore;
                    g[3] = maxAisleScore;
                    maxAisleScore = aisleCounter;
                }
                
            }
            
        }
    }
    
    return maxGroupScore;
}

int main(){
    
    int numOfRows, seatsPerRow, numOfAisles;
    int rowNumArray[100];
    for(int i = 0; i < 100; i++){
        rowNumArray[i] = i+1;
    }
    printf("Number of rows: ");
    scanf("%d", &numOfRows);
    printf("Number of seats per row: ");
    scanf("%d", &seatsPerRow);
    printf("Number of aisles: ");
    scanf("%d", &numOfAisles);
    int aisles[5];
    
    for(int i = 0; i < numOfAisles; i++){
        printf("Add aisle %d after seat: ", i+1);
        scanf("%d", &aisles[i]);
    }
    
    char plane[110][40];
    plane[0][0] = ' ';
    
    for(int i = 0; i < numOfRows + 1; i++){
        for(int j = 0; j < seatsPerRow + numOfAisles; j++){
            plane[i][j] = '-';
        }
    }
    
    
    
    int ais = 0;
    for(int i = 0; i < numOfRows + 1; i++){
        ais = 0;
        for(int j = 0; j < seatsPerRow + numOfAisles; j++){
            if(plane[i][j] == '-' && j == aisles[ais] + ais){
                plane[i][j] = '|';

                j++;
                ais++;
                
            }
        }
    }
    
    
    char alphabet = 65;
    for(int i = 0; i < numOfRows; i++){
        for(int j = 0; j < seatsPerRow + numOfAisles; j++){
            if(plane[i][j] == '-' && i == 0){
                plane[i][j] = alphabet;
                alphabet++;
            }
        }
    }
    
    show_plane(numOfRows, rowNumArray, numOfAisles, seatsPerRow, plane);
    
    int peopleNum = 0;
    int numOfReservedSeats = 0;
    
    while(peopleNum != -1){
        int n = 0;
        double** eachSeat = malloc((numOfRows*seatsPerRow) * sizeof(double*));
        for(int i = 0; i < numOfRows*seatsPerRow; i++){
            *(eachSeat + i) = malloc(3 * sizeof(double));
        }
        
        printf("The number of people in the reservation\n (0: print current reservations, -1: exit): ");
        scanf("%d", &peopleNum);
        
        
        //SHOWING PLANE WHEN USER ENTERED 0
        if(peopleNum == 0){
            show_plane(numOfRows, rowNumArray, numOfAisles, seatsPerRow, plane);
        }else if(numOfReservedSeats == numOfRows * seatsPerRow){
            printf("\nNo available seats for the requested reservation!\n\n");
            continue;
        }
        
        
        // IF PEOPLENUM === 1 AND IT IS THE FIRST REQUEST
        if(peopleNum == 1 && numOfReservedSeats == 0){
            plane[1][0] = '+';
            printf("Reserved seats: 1A\n");
            show_plane(numOfRows, rowNumArray, numOfAisles, seatsPerRow, plane);
            plane[1][0] = 'X';
            numOfReservedSeats++;
            continue;
        }
        
        // PEOPLENUM IS EQUAL TO ONE BUT IT IS NOT THE FIRST REQUEST
        else if(peopleNum == 1){
            n = 0;
            for(int i = 1; i < numOfRows + 1; i++){
                for(int j = 0; j < seatsPerRow + numOfAisles; j++){
                    if(plane[i][j] == '-'){
                        
                        double* s = malloc(3 * sizeof(double));
                        s[0] = i;
                        s[1] = j + 1;
                        s[2] = 0;
                        double scoreOfSeat = score(s, plane, numOfRows, numOfAisles, seatsPerRow);
                        s[2] = scoreOfSeat;
                        eachSeat[n][0] = s[0];
                        eachSeat[n][1] = s[1];
                        eachSeat[n][2] = s[2];
                        n++;
                        
                    }
                }
            }
            
            double max = findMax(numOfRows, seatsPerRow, eachSeat, numOfReservedSeats);

            for(int i = 0; i < numOfRows * seatsPerRow; i++){
                
            if(eachSeat[i][2] == max){
                int i1 = eachSeat[i][0];
                int j1 = eachSeat[i][1] - 1;
                plane[ i1 ] [ j1 ] = '+' ;
                printf("Reserved seats: %d%c\n", rowNumArray[i1] - 1, plane[0][j1]);
                show_plane(numOfRows, rowNumArray, numOfAisles, seatsPerRow, plane);
                plane[ i1 ] [ j1 ] = 'X' ;
                numOfReservedSeats++;
                break;
            }
                
        }

        }
        
        //GROUP RESERVATION
        else if(peopleNum > 1){
            double g[4];
            g[0] = 0;
            g[1] = 0;
            g[2] = 0;
            g[3] = 0;
            double result = seat_block_score(peopleNum, plane, numOfRows, numOfAisles, seatsPerRow, aisles, g);
            
            if(result == -9999999999){
                printf("\nNo available seats for the requested reservation!\n\n");
                continue;
            }
            else{
                numOfReservedSeats+=peopleNum;
                int g3 = g[3];
                int g0 = g[0];
                int g1 = g[1];
                for(int k = 0; k < peopleNum + g3; k++){

                    if(plane[g0][g1 - k] == '-'){
                        plane[g0][g1 - k] = '+';
                    }

                }
                printf("Reserved seats:");
                for(int k = peopleNum+g3-1; k >= 0; k--) {
                    if (plane[g0][g1 - k] == '+'){
                        printf(" %d%c", g0, plane[0][g1-k]);
                    }
   
                }
                printf("\n");
                
                show_plane(numOfRows, rowNumArray, numOfAisles, seatsPerRow, plane);
                
                for(int k = 0; k < peopleNum + g3; k++){
                    
                    if(plane[g0][g1 - k] == '+'){
                        plane[g0][g1 - k] = 'X';
                    }
                    
                }
                printf("\n");
                
            }
            
        }

    }

    return 0;
}

