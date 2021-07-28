#include<iostream>
using namespace std;

#define MAX_INT 2147483647

struct vertex;

struct edge
{
    int v;
    int length;
    bool isOn;

    edge()
    {
        isOn = false;
    }
};


class queueElement
{
public:
    edge element;
    queueElement* pointerToMember;
};

class queue
{
public:
    queueElement* firstMember;
    queueElement* lastMember;

    queue()
    {
        firstMember = NULL;
        lastMember = NULL;
    }

    void add(edge queueMember)
    {
        queueElement* addedEdge = new queueElement;
        addedEdge->element = queueMember;
        addedEdge->pointerToMember = NULL;
        if (lastMember != NULL) lastMember->pointerToMember = addedEdge;
        lastMember = addedEdge;
        if (firstMember == NULL) firstMember = lastMember;
    }

    bool isQueueEmpty()
    {
        if (firstMember == NULL) return true;
        else return false;
    }
};

struct vertex
{
    int distanceStart, distanceEnd, heapPosition, distance;
    queue edges;

    vertex()
    {
        distanceStart = MAX_INT;
        distanceEnd = MAX_INT;
        distance = MAX_INT;
        heapPosition = MAX_INT;
    }
};

void createHeapMin(int tab[], int i, int n, vertex* verticesArray)
{
    int minimumValue;
    int leftChild = 2 * i;
    int rightChild = 2 * i + 1;

    if (leftChild <= n && verticesArray[tab[i]].distance > verticesArray[tab[leftChild]].distance)
    {
        minimumValue = leftChild;
    }
    else minimumValue = i;

    if (rightChild <= n && verticesArray[tab[minimumValue]].distance > verticesArray[tab[rightChild]].distance)
    {
        minimumValue = rightChild;
    }

    if (minimumValue != i)
    {
        int temp = tab[minimumValue];
        tab[minimumValue] = tab[i];
        tab[i] = temp;

        verticesArray[tab[i]].heapPosition = i;
        verticesArray[tab[minimumValue]].heapPosition = minimumValue;

        createHeapMin(tab, minimumValue, n, verticesArray);
    }
}

void changeValueMin(int tab[], int i, int n, vertex* verticesArray)
{
    int minimumValue;

    while (i > 1 && verticesArray[tab[i / 2]].distance > verticesArray[tab[i]].distance)
    {
        int temp = tab[i / 2];
        tab[i / 2] = tab[i];
        tab[i] = temp;

        verticesArray[tab[i]].heapPosition = i;
        verticesArray[tab[i / 2]].heapPosition = i / 2;

        i = i / 2;
    }

    int leftChild = 2 * i;
    int rightChild = 2 * i + 1;
    if (leftChild <= n && verticesArray[tab[i]].distance > verticesArray[tab[leftChild]].distance)
    {
        minimumValue = leftChild;
    }
    else minimumValue = i;

    if (rightChild <= n && verticesArray[tab[minimumValue]].distance > verticesArray[tab[rightChild]].distance)
    {
        minimumValue = rightChild;
    }

    if (minimumValue != i)
    {
        int temp = tab[minimumValue];
        tab[minimumValue] = tab[i];
        tab[i] = temp;

        verticesArray[tab[i]].heapPosition = i;
        verticesArray[tab[minimumValue]].heapPosition = minimumValue;

        changeValueMin(tab, minimumValue, n, verticesArray);
    }
}

void dijkstraAlgorithm(vertex* verticesArray, int* heap, int numbersOfVertices, bool whichVertex, int startIndex, edge* negated)
{
    verticesArray[startIndex].distance = 0;
    changeValueMin(heap, verticesArray[startIndex].heapPosition, numbersOfVertices, verticesArray);

    while (verticesArray[heap[1]].distance < MAX_INT)
    {
        int current = heap[1];

        if (whichVertex == true)
        {
            verticesArray[current].distanceStart = verticesArray[current].distance;
        }
        else
        {
            verticesArray[current].distanceEnd = verticesArray[current].distance;
        }

        verticesArray[current].distance = MAX_INT;
        changeValueMin(heap, 1, numbersOfVertices, verticesArray);

        queueElement* iterator = verticesArray[current].edges.firstMember;
        while (iterator != NULL)
        {
            int candidate = iterator->element.v;
            if (whichVertex == true)
            {
                if (verticesArray[candidate].distanceStart == MAX_INT)
                {
                    if (verticesArray[candidate].distance > verticesArray[current].distanceStart + iterator->element.length)
                    {
                        if (&(iterator->element) != negated)
                        {
                            verticesArray[candidate].distance = verticesArray[current].distanceStart + iterator->element.length;
                            changeValueMin(heap, verticesArray[candidate].heapPosition, numbersOfVertices, verticesArray);
                        }
                    }
                }
            }
            else
            {
                if (verticesArray[candidate].distanceEnd == MAX_INT)
                {
                    if (verticesArray[candidate].distance > verticesArray[current].distanceEnd + iterator->element.length)
                    {
                        verticesArray[candidate].distance = verticesArray[current].distanceEnd + iterator->element.length;
                        changeValueMin(heap, verticesArray[candidate].heapPosition, numbersOfVertices, verticesArray);
                    }
                }
            }
            iterator = iterator->pointerToMember;
        }
    }
}

void BFS(vertex* verticesArray, int startIndex, int endIndex)
{
    if (startIndex == endIndex) return;

    queueElement* iterator = verticesArray[startIndex].edges.firstMember;
    while (iterator != NULL)
    {
        int candidate = iterator->element.v;
        if (verticesArray[startIndex].distanceEnd == verticesArray[candidate].distanceEnd + iterator->element.length)
        {
            iterator->element.isOn = true;
            BFS(verticesArray, candidate, endIndex);
            return;
        }

        iterator = iterator->pointerToMember;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    int numberOfVertices, numberOfEdges;
    int minimum = MAX_INT;
    cin >> numberOfVertices >> numberOfEdges;
    int* heapArray = new int[numberOfVertices + 1];

    vertex* vertexArray = new vertex[numberOfVertices];
    for (int i = 0; i < numberOfEdges; i++)
    {
        int x = 0, y = 0, z = 0;
        cin >> x >> y >> z;
        edge temp;
        temp.v = y;
        temp.length = z;
        vertexArray[x].edges.add(temp);
        temp.v = x;
        vertexArray[y].edges.add(temp);
    }

    for (int i = 1; i < numberOfVertices + 1; i++)
    {
        heapArray[i] = i - 1;
        vertexArray[i - 1].heapPosition = i;
    }

    for (int i = (numberOfVertices + 1) / 2; i > 0; i--)
    {
        createHeapMin(heapArray, i, numberOfVertices, vertexArray);
    }

    dijkstraAlgorithm(vertexArray, heapArray, numberOfVertices, false, numberOfVertices - 1, NULL);

    BFS(vertexArray, 0, numberOfVertices - 1);
    for (int i = 0; i < numberOfVertices; i++)
    {
        queueElement* iterator = vertexArray[i].edges.firstMember;
        while (iterator != NULL)
        {
            if (iterator->element.isOn == true)
            {
                for (int i = 0; i < numberOfVertices; i++)
                {
                    vertexArray[i].distanceStart = MAX_INT;
                }
                dijkstraAlgorithm(vertexArray, heapArray, numberOfVertices, true, 0, &(iterator->element));
            }

            iterator->element.isOn = false;
            iterator = iterator->pointerToMember;

            if (vertexArray[numberOfVertices - 1].distanceStart < minimum)
            {
                minimum = vertexArray[numberOfVertices - 1].distanceStart;
            }
        }
    }

    if (minimum < MAX_INT)
        cout << vertexArray[0].distanceEnd << " " << minimum;
    else
        cout << "#";

    delete[] heapArray;
    delete[] vertexArray;

    return 0;
}