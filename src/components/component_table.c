// Maybe create an api

typedef struct Cell {
    int length;
    int align;
    char text[128]
} Cell;

typedef struct Row {
    int bold;
    Cell* cells;
} Row;

void table(Row* rows)
{
    for (int i = 0; rows[i] != NULL; i++)
    {
        for (int j = 0; rows[i].cells[j] != NULL; j++)
        {
            // if i zero, we are going to render the 
            // if j zero render | laterally
        }
    }
    // render the closing bracket
}



// A variable length struct with row

// A varibale length struct called colun 