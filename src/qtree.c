#include "qtree.h"
#include <math.h>

double compute_RMSE(Image *image, double average, int width, int height, int starting_col, int starting_row){
    double sum = 0.0;
    int count = 0;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            sum += pow((image->pixels[(starting_row + i) * image->width + (starting_col + j)]) - average, 2);
            // Identical reasoning given below for obtaining the average intensity 
            count++;
        }
    }

    double new_average = sum / count; 
    double RMSE = sqrt(new_average);
    return RMSE;
}

QTNode *create_quadtree_helper(Image *image, double max_rmse, int width, int height, int starting_row, int starting_col){
    QTNode *node = malloc(sizeof(QTNode));

    node->width = width;
    node->height = height;
    node->starting_row = starting_row;
    node->starting_col = starting_col;

    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < node->height; i++) {
        for (int j = 0; j < node->width; j++) {
            sum += image->pixels[(node->starting_row + i) * image->width + (node->starting_col + j)]; 
            // Similar to getting a specific element given row and column, but we are accumulating values, not just retreiving a value.
            // This signals nested for loops, since we want to iterate a finite number of times (ie. based on number of rows and columns/dimensions of the image).
            // So treat this as a 2D array and write a summation function as we would normally do: 
            // Copy+paste the original formula for retrieving value but now we know the row and column values will always be incremented - 
            // The starting_row stays the same for a while and the column changes by 1 everytime, then increment the starting_row by 1 for the next row and repeat 
            // This is just a combination of HOW we sum elements in a 2D array and WHERE we actually start the summation in the 1D array
            count++;
        }
    }

    double average = sum / count;
    unsigned char intensity = (unsigned char)average;
    node->intensity = intensity;


    double RMSE = compute_RMSE(image, average, width, height, starting_col, starting_row);

    if(RMSE <= max_rmse){
        node->node_type = 'L';
        node->child1 = NULL;
        node->child2 = NULL;
        node->child3 = NULL;
        node->child4 = NULL;
    } else {
        node->node_type = 'N';
        if(width == 1){
            node->child2 = NULL;
            node->child4 = NULL;
            node->child1 = create_quadtree_helper(image, max_rmse, 1, height / 2, starting_row, starting_col);
            node->child3 = create_quadtree_helper(image, max_rmse, 1, height - (height / 2), starting_row + (height / 2), starting_col);
        } else if (height == 1){
            node->child3 = NULL;
            node->child4 = NULL;
            node->child1 = create_quadtree_helper(image, max_rmse, width / 2, 1, starting_row, starting_col);
            node->child2 = create_quadtree_helper(image, max_rmse, width - (width / 2), 1, starting_row, starting_col + width / 2);
        } else {
            node->child1 = create_quadtree_helper(image, max_rmse, width / 2, height / 2, starting_row, starting_col);
            node->child2 = create_quadtree_helper(image, max_rmse, width - (width / 2), height / 2, starting_row, starting_col + width / 2);
            // I noticed from the Google Doc with the given test case that Child #2 of node C has starting column of 96, even though the given formula
            // for the starting column is width / 2. After physically drawing out the scenario, immediately it makes sense that Node E wouldnt start at
            // 32 (which is width / 2) because Node C itself already starts AT column 64 so why would it go backwards? This acts like a y-intercept
            // so we need to add the current starting_column of the node to width / 2
            // This line of reasoning is identical for child 3 and 4 but also considering the starting row
            
            // Also, notice that the new width for child2 is not simply width / 2. This only works for even numbers.
            // Like for 256x256: we expect the row and width to be 128x128 for both child1 and child2.
            // Just using a small odd number, we can quickly derive a formula to calculate the correct width for child2
            // For example, if we have 3 columns. child1's new width is 7 / 2 is 3 because of integer width. 
            // If child2's new width is also 7 / 2, then we are not fulling gathering all columns (child1 and child2 only cover 6 columns)
            // To fix this, go back to very basic algebra. Let x = child2 width. Then, x + CHILD_1_WIDTH = TOTAL_WIDTH. Solving for x: TOTAL_WIDTH - CHILD_1_WIDTH = x 
            // Now let CHILD_1_WIDTH = 3 and TOTAL_WIDTH = 7, then 7 - 3 = x, x = 4
            // Of course, this still works for even numbers: let CHILD_1_WIDTH = 128 and TOTAL_WIDTH = 256, then 256 - 128 = 128
            // We need to do this with rows as needed for child3 and child4.
            node->child3 = create_quadtree_helper(image, max_rmse, width / 2, height - (height / 2),  starting_row + height / 2, starting_col);
            node->child4 = create_quadtree_helper(image, max_rmse, width - (width / 2), height - (height / 2), starting_row + height / 2,  starting_col + width / 2);
        }
    }

    return node;

}

QTNode *create_quadtree(Image *image, double max_rmse) {
    QTNode *root = create_quadtree_helper(image, max_rmse, image->width, image->height, 0, 0);
    return root;
}

QTNode *get_child1(QTNode *node) {
    return node->child1;
}

QTNode *get_child2(QTNode *node) {
    return node->child2;
}

QTNode *get_child3(QTNode *node) {
    return node->child3;
}

QTNode *get_child4(QTNode *node) {
    return node->child4;
}

unsigned char get_node_intensity(QTNode *node) {
    return node->intensity;
}

void delete_quadtree(QTNode *root) {
    if(root->child1 != NULL){
        delete_quadtree(root->child1);
    }
    if(root->child2 != NULL){
        delete_quadtree(root->child2);
    }
    if(root->child3 != NULL){
        delete_quadtree(root->child3);
    }
    if(root->child4 != NULL){
        delete_quadtree(root->child4);
    }

    free(root);
}

// void save_qtree_as_ppm_helper(QTNode *root, int* intensity_values, int width){
//     char type = root->node_type;

//     if(root == NULL){
//         return;
//     } else if (type == 'L'){
//         for(int i = 0; i < root->height; i++){
//             for(int j = 0; j < root->width; j++){
//                 intensity_values[(root->starting_row + i) * width + (root->starting_col + j)] = root->intensity;
//             }
//         }
//     } else {
//         save_qtree_as_ppm_helper(root->child1, intensity_values, width);
//         save_qtree_as_ppm_helper(root->child2, intensity_values, width);
//         save_qtree_as_ppm_helper(root->child3, intensity_values, width);
//         save_qtree_as_ppm_helper(root->child4, intensity_values, width);
//     }
// }

void save_qtree_as_ppm_helper(QTNode *root, int* intensity_values, int width){
    if(root == NULL){
        return;
    }

    if(root->node_type == 'L'){
        for(int i = 0; i < root->height; i++){
            for(int j = 0; j < root->width; j++){
                int starting_row = i + root->starting_row;
                int starting_col = j + root->starting_col;
                int intensity = root->intensity;
                intensity_values[starting_row * width + starting_col] = intensity;
            }
        }
    } else {
        save_qtree_as_ppm_helper(root->child1, intensity_values, width);
        save_qtree_as_ppm_helper(root->child2,  intensity_values, width);
        save_qtree_as_ppm_helper(root->child3,  intensity_values, width);
        save_qtree_as_ppm_helper(root->child4, intensity_values, width);
    }


}

void save_qtree_as_ppm(QTNode *root, char *filename) {
    FILE *write = fopen(filename, "w");

    if(write == NULL){
        return;
    }

    fprintf(write, "%s\n", "P3");
    fprintf(write, "%d %d\n", root->width, root->height);
    fprintf(write, "255\n");

    int *intensity_values = malloc(sizeof(int) * root->width * root->height);

    save_qtree_as_ppm_helper(root, intensity_values, root->width);

    for(int i = 0; i < root->width * root->height; i++){
        fprintf(write, "%d %d %d \n", intensity_values[i], intensity_values[i], intensity_values[i]);
    }

    free(intensity_values);
    fclose(write);



    //  FILE *write = fopen(filename, "w");
    //  int *intensity_values = malloc(sizeof(int) * root->width * root->height);
    //  save_qtree_as_ppm_helper(root, intensity_values, root->width);
    //  fprintf(write, "%s\n", "P3");
    //  fprintf(write, "%d %d\n", root->width, root->height);
    //  fprintf(write,"%d\n", 255);
    //  for(int i = 0; i < root->height*root->width; i++) {
    //     fprintf(write, "%d %d %d\n", intensity_values[i], intensity_values[i], intensity_values[i]);
    // }
    // free(intensity_values);
    // fclose(write);
}

QTNode *load_preorder_qt_helper(FILE *read, int level){

    QTNode *root = malloc(sizeof(QTNode));

    char type = fgetc(read);

    if(type == EOF){
   
        free(root);
        return NULL;
    }

    int intensity;
    int starting_row, height, starting_col, width; 

    fscanf(read, " %d %d %d %d %d\n", &intensity, &starting_row, &height, &starting_col, &width);

    root->node_type = type;
    root->intensity = (unsigned char)intensity; 
    root->starting_row = starting_row;
    root->height = height;
    root->starting_col = starting_col;
    root->width = width;

    if(type == 'N'){
        if(height == 1){
            root->child1 = load_preorder_qt_helper(read, level + 1);
            root->child2 = load_preorder_qt_helper(read, level + 1);
            root->child3 = NULL;
            root->child4 = NULL; 
        } else if(width == 1){
            root->child2 = NULL;
            root->child4 = NULL;
            root->child1 = load_preorder_qt_helper(read, level + 1);
            root->child3 = load_preorder_qt_helper(read, level + 1);
        } else {
            root->child1 = load_preorder_qt_helper(read, level + 1);
            root->child2 = load_preorder_qt_helper(read, level + 1);
            root->child3 = load_preorder_qt_helper(read, level + 1);
            root->child4 = load_preorder_qt_helper(read, level + 1);

        }

    } else if (type == 'L') {
        root->child1 = NULL;
        root->child2 = NULL;
        root->child3 = NULL;
        root->child4 = NULL; 
    } 
    return root;
}

QTNode *load_preorder_qt(char *filename) {
    FILE *read = fopen(filename, "r");
    if(read == NULL){
        return NULL;
    }
    QTNode *root = load_preorder_qt_helper(read, 0);
    fclose(read);
    return root;
}

void save_preorder_qt_helper(QTNode *root, FILE *write){
    // Immediately, I noticed that without a helper function such as this one, I would always fopen() a file and from the Google Doc, doing so overwrites it
    // This is where the helper function and pointers are useful as I can just pass in a singular write file and edit it accordingly
    if(root != NULL){
        fprintf(write, "%c %hhu %d %d %d %d\n", root->node_type, root->intensity, 
        root->starting_row, root->height, root->starting_col, root->width);
        save_preorder_qt_helper(root->child1, write);
        save_preorder_qt_helper(root->child2, write);
        save_preorder_qt_helper(root->child3, write);
        save_preorder_qt_helper(root->child4, write);
    } else {
        return;
    }
}

void save_preorder_qt(QTNode *root, char *filename) {
    FILE *write = fopen(filename, "w");
    save_preorder_qt_helper(root, write);
    fclose(write);
}
