#include "image.h"
#include "qtree.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
   
    double max_rmse = 25;
    Image *image = load_image("./images/originals/building1.ppm");
    QTNode *root = create_quadtree(image, max_rmse);
//    save_preorder_qt(root, "tests/output/save_preorder_GIVEN.txt");
    root = create_quadtree(image, 25);
    save_qtree_as_ppm(root, "tests/output/save_qtree_as_ppm1.ppm");



    // printf("%c %hhu %d %d %d %d\n", root->node_type, 
    //  root->intensity, root->starting_row, root->height, root->starting_col, root->width);

    // printf("%c %hhu %d %d %d %d\n", root->child1->node_type, 
    // root->child1->intensity, root->child1->starting_row, root->child1->height, root->child1->starting_col, root->child1->width);

    // printf("%c %hhu %d %d %d %d\n", root->child1->child1->node_type, 
    // root->child1->child1->intensity, root->child1->child1->starting_row, root->child1->child1->height, root->child1->child1->starting_col, root->child1->child1->width);

    // printf("%c %hhu %d %d %d %d\n", root->child1->child2->node_type, 
    // root->child1->child2->intensity, root->child1->child2->starting_row, root->child1->child2->height, root->child1->child2->starting_col, root->child1->child2->width);
   
    // printf("%c %hhu %d %d %d %d\n", root->child1->child2->child1->node_type, 
    // root->child1->child2->child1->intensity, root->child1->child2->child1->starting_row, root->child1->child2->child1->height, root->child1->child2->child1->starting_col, root->child1->child2->child1->width);
   
    // printf("%c %hhu %d %d %d %d\n", root->child1->child2->child2->node_type, 
    // root->child1->child2->child2->intensity, root->child1->child2->child2->starting_row, root->child1->child2->child2->height, root->child1->child2->child2->starting_col, root->child1->child2->child2->width);
   
   
     delete_quadtree(root);
     delete_image(image);   
    return 0;
}