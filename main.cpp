#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

struct square{
    int row;
    int column;
    int value;
    bool original;
    vector<int> possible;
    int box;
};

class gameboard{
    public:
    vector<vector<square> > squares;

    gameboard(vector<vector<square>> temp){
        squares = temp;
    }

    void solve() {
        while(!check_done()) {
            insert();
            if(check_duplicates())
            {
                revert();
            }
        }
    }

    bool check_duplicates()
    {
        /*
        ideas for check_duplicates:
        1. could split up into three seperate parts, first we search through and put into boxes, then rows, then columns
            if any duplicates are discovered we immediately return true. runtime inefficient because we'd be iterating over squares
            multiple times. However, the gameboard isn't the big and this would be potentially the easiest way
        2. have one iterating loop that manages to do all of this at once, would be great for runtime, significantly harder to conceptualize.
        3. some kind of hybrid, I think I could do rows and columns without splitting things up too much, but boxes are more confusing
        */

       //starting idea 1
       //check boxes:
       vector<int> dummy;
       vector<vector<int>> boxes;
       vector<vector<int>> row;
       vector<vector<int>> col;
       for(int i =0; i < 9; ++i) {
           boxes.push_back(dummy);
           row.push_back(dummy);
           col.push_back(dummy);
       }
       for(vector<square> i : squares)
       {
           for(square j : i)
           {
               boxes[j.box].push_back(j.value);
               row[j.row].push_back(j.value);
               col[j.column].push_back(j.value);
           }
       }
        
        for(auto i : boxes)
        {
           for(int j = 0; j < 9; ++j)
           {
               for(int k = 0; k < 9; ++k) 
               {
                   if(j != k && i[j] == i[k] && i[j] != 0)
                   {
                       return true;
                   }
               }
           }
       }

       //rows
        for(auto i : row)
        {
            for(int j = 0; j < 9; ++j)
           {
               for(int k = 0; k < 9; ++k) 
               {
                   if(j != k && i[j] == i[k] && i[j] != 0)
                   {
                       return true;
                   }
               }
           }
        }

        //columns
        for(auto i : col)
        {
            for(int j = 0; j < 9; ++j)
           {
               for(int k = 0; k < 9; ++k) 
               {
                   if(j != k && i[j] == i[k] && i[j] != 0)
                   {
                       return true;
                   }
               }
           }
        }




    
    return false;
    }

    void insert() {
        int indexerCol = 0;
        int indexerRow = 0;
        while(squares[indexerRow][indexerCol].value != 0) 
        {
            if(++indexerCol >= 9)
            {
                ++indexerRow;
                indexerCol = 0;
            }
        }
        squares[indexerRow][indexerCol].value = squares[indexerRow][indexerCol].possible[0];
    }
    //needs to be completely reworked, the current way it only steps back one step, need a way to get it to recursively go back?
    void revert(){
        int indexR = 0;
        int indexC = 0;
        bool lastSquare = check_done();
        if(!lastSquare) {
            while(squares[indexR][indexC].value != 0) {
                if(++indexC >= 9)
                {
                    ++indexR;
                    indexC = 0;
                }
            }
            --indexC;
            if(indexC == -1)
            {
                --indexR;
                indexC = 8;
            }
        
        
            while(squares[indexR][indexC].original)
            {
                --indexC;
                if(indexC == -1)
                {
                    --indexR;
                    indexC = 8;
                }
            }
        }
        else{
            indexR = 8;
            indexC = 8;
        }
        squares[indexR][indexC].value = 0;
        if(squares[indexR][indexC].possible.size() > 1) {
            squares[indexR][indexC].possible.erase(squares[indexR][indexC].possible.begin());
        }
        else{
            squares[indexR][indexC].possible = {1,2,3,4,5,6,7,8,9};
            revert();
        }

    }

    void print(ostream &output) {
        for(auto i : squares)
        {
            for(auto j : i)
            {
                output << j.value << " ";
            }
            output << endl;
        }
        cout << endl;
    }

    bool check_done() {
        for(auto i : squares) {
            for(auto j : i) {
                if(j.value == 0) {
                    return false;
                }
            }
        }
        return true;
    }



};





int main() {
    string filename;
    cout << "Filename:";
    cin >> filename;
    cout << "What file should this be saved to?" << endl;
    string fileout;
    cin >> fileout;

    ifstream board(filename.c_str());
    ofstream output(fileout.c_str());
    vector<vector<square>> sudoku;
    int one;
    int row = 0;
    while(board >> one)
    {
        vector<int> p = {one};
        bool og = true;
        if(one == 0){
            og = false;
            p = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        }
        square unit;
        unit.row = row;
        unit.column = 0;
        unit.value = one;
        unit.original = og;
        unit.possible = p;
        vector<square> temp;
        temp.push_back(unit);
        for(int i = 1; i < 9; ++i) {
            int value;
            board >> value;
            p = {value};
            og = true;
            if(value == 0) {
                og = false;
                p = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            }
            unit.column = i;
            unit.original = og;
            unit.value = value;
            unit.possible = p;
            temp.push_back(unit);
        }
        sudoku.push_back(temp);
        ++row;
    }
    
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (sudoku.at(i).at(j).row < 3 && sudoku.at(i).at(j).column < 3)
            {
                sudoku.at(i).at(j).box = 0;
            }
            else if (sudoku.at(i).at(j).row < 3 && sudoku.at(i).at(j).column < 6)
            {
                sudoku.at(i).at(j).box = 1;
            }
            else if (sudoku.at(i).at(j).row < 3 && sudoku.at(i).at(j).column < 9)
            {
                sudoku.at(i).at(j).box = 2;
            }
            else if (sudoku.at(i).at(j).row < 6 && sudoku.at(i).at(j).column < 3)
            {
                sudoku.at(i).at(j).box = 3;
            }
            else if (sudoku.at(i).at(j).row < 6 && sudoku.at(i).at(j).column < 6)
            {
                sudoku.at(i).at(j).box = 4;
            }
            else if (sudoku.at(i).at(j).row < 6 && sudoku.at(i).at(j).column < 9)
            {
                sudoku.at(i).at(j).box = 5;
            }
            else if (sudoku.at(i).at(j).row < 9 && sudoku.at(i).at(j).column < 3)
            {
                sudoku.at(i).at(j).box = 6;
            }
            else if (sudoku.at(i).at(j).row < 9 && sudoku.at(i).at(j).column < 6)
            {
                sudoku.at(i).at(j).box = 7;
            }
            else if (sudoku.at(i).at(j).row < 9 && sudoku.at(i).at(j).column < 9)
            {
                sudoku.at(i).at(j).box = 8;
            }
        }
    }
    

    /*
    for(auto i : sudoku)
    {
        for(auto j : i)
        {
            if(j.row < 3) {
                if(j.column < 3) {
                    j.box = 0;
                }
                else if(j.column < 6) {
                    j.box = 1;
                }
                else {j.box = 2;}
            }
            else if(j.row < 6) {
                if(j.column < 3) {
                    j.box = 3;
                }
                else if(j.column < 6) {
                    j.box = 4;
                }
                else {j.box = 5;}
            }
            else{
                if(j.column < 3) {
                    j.box = 6;
                }
                else if(j.column < 6) {
                    j.box = 7;
                }
                else {j.box = 8;}
            }
        }
    }
    */
    

    gameboard Final_Solution(sudoku);
    Final_Solution.solve();
    Final_Solution.print(output);

    board.close();
    output.close();
    return 0;
}