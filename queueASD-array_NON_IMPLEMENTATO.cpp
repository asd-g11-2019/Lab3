#include "queueASD-array.h"

using namespace queue;


/****************************************************************/
/* 			FUNZIONI SULLE CODE 			*/
/****************************************************************/

/****************************************************************/
Queue queue::createEmpty()
{
    Queue q;
    q.size = 0;
    q.maxsize = 0;
    q.queue = nullptr;
    return q;
}

void resize (Queue& l, bool expand) {
    int newSize = l.maxsize; // Metto da parte la dimensione nuova

    if (expand) {// Se voglio espandere
        if (++l.size > l.maxsize) // Incremento il numero degli elementi e se supera la dimensione
            newSize += BLOCKDIM; // Incremento la nuova dimensione di BLOCKDIM
    } else { // Se voglio contrarre
        if (--l.size+BLOCKDIM <= l.maxsize) // Decremento il numero degli elementi e se tra esso e la dimensione ci sta un BLOCKDIM
            newSize -= BLOCKDIM; // Decremento la nuova dimensione di BLOCKDIM
    }

    if (newSize == l.maxsize) // Se la dimensione non e' cambiata (ovvero non devo ridimensionare l'array)
        return; // Ritorno

    l.maxsize = newSize; // Altrienti imposto la dimensione nuova

    Elem* queue = new Elem[l.maxsize]; // Inizializzo il nuovo array

    if (expand)
        for (int i=0; i<l.size-1; i++) // Per ogni elemento da copiare
            queue[i] = l.queue[i]; // Copio dal vecchio array al nuovo array
    else 
        for (int i=0; i<l.maxsize; i++) // Per ogni elemento da copiare
            queue[i] = l.queue[i+i]; // Copio dal vecchio array al nuovo array

    if (l.queue != nullptr)
        delete[] l.queue; // Elimino il vecchio array

    l.queue = queue; // Imposto il nuovo array nella queue
}

/****************************************************************/
void queue::enqueue(Elem e, Queue& l) // aggiunge e in coda
{
    if (l.size+1 > l.maxsize) {
        l.maxsize += BLOCKDIM;
        Elem* queue = new Elem[l.maxsize]; // Inizializzo il nuovo array
    
        for (int i=0; i<l.size; i++) // Per ogni elemento da copiare
            queue[i] = l.queue[i]; // Copio dal vecchio array al nuovo array
    
        if (l.queue != nullptr)
            delete[] l.queue; // Elimino il vecchio array

        l.queue = queue;
    }

    l.queue[l.size++] = e;
}


/****************************************************************/
Elem queue::dequeue(Queue& l) // rimuove il primo elemento e lo restituisce
{
    if (isEmpty(l))
        return EMPTYELEM;
    
    Elem e = l.queue[0];

    Elem* oldQueue = l.queue;

    if (l.size+BLOCKDIM-1 <= l.maxsize) {
        l.maxsize -= BLOCKDIM;
        Elem* queue = new Elem[l.maxsize]; // Inizializzo il nuovo array

        l.queue = queue;
    }

    for (int i=0; i<l.size-1; i++) // Per ogni elemento da copiare
        l.queue[i] = oldQueue[i+1]; // Copio dal vecchio array al nuovo array

    l.size--;

    return e;
}


/****************************************************************/
Elem queue::first(Queue& l) // restituisce il primo elemento
{
    if (isEmpty(l))
        return EMPTYELEM;
    
    return l.queue[0];
}


/****************************************************************/
bool queue::isEmpty(const Queue& l)
{
    return (l.size == 0);
}



/****************************************************************/
Queue readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    return readFromStream(ifs);
}


/****************************************************************/
Queue readFromStdin()
{
        cout << "\nInserire una sequenza di numeri separati da spazi seguiti da " << FINEINPUT << " per terminare\n";
        return readFromStream((std::cin));
}


/****************************************************************/
Queue readFromStream(istream& str)
{
    Queue l = createEmpty();        
    int dato;
    str>>dato;
    if (!str) throw runtime_error("Errore inserimento dati\n");
    while (dato!= FINEINPUT)  // assumiamo che il segnale di fine input nel file sia  FINEINPUT
        {     
        enqueue(dato, l);   // aggiunge l'elemento alla fine della coda
        str>>dato;
        if (!str) throw runtime_error("Errore inserimento dati\n");
        }
return l;
}

/****************************************************************/
void print(const Queue& l)
{
cout << endl;
for (int i=0; i<l.size; ++i)
   cout << l.queue[i] << "; "; 
cout << endl;
}
