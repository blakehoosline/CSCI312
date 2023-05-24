#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M 18
#define MAX_NODES (1 << M)

int binary_search(int *arr, int n, int target)
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return low;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <node ID>\n", argv[0]);
        return 1;
    }
    int node_id = atoi(argv[1]);
    FILE *fp = fopen("nodeIDs", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: could not open nodeIDs file\n");
        return 1;
    }
    int node_ids[MAX_NODES];
    int num_nodes = 0;
    while (num_nodes < MAX_NODES && fscanf(fp, "%d", &node_ids[num_nodes]) == 1)
    {
        num_nodes++;
    }
    fclose(fp);

    // computing the finger table
    int finger_table[M][2];
    for (int i = 0; i < M; i++)
    {
        finger_table[i][0] = (node_id + (1 << i)) % MAX_NODES;
        finger_table[i][1] = node_id;
        int j = binary_search(node_ids, num_nodes, finger_table[i][0]);
        if (j < num_nodes)
        {
            finger_table[i][1] = node_ids[j];
        }
    }

    // print the finger table
    printf("Finger table for node %d:\n", node_id);
    for (int i = 0; i < M; i++)
    {
        int start = finger_table[i][0];
        int end = (start + (1 << i) - 1) % MAX_NODES;
        printf("[%d,%d]: %d\n", start, end, finger_table[i][1]);
    }

    return 0;
}