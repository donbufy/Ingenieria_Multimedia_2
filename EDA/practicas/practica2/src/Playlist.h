// Práctica 2 — Reproductor de música
// Plantilla: completa la declaración del TAD Playlist
// siguiendo la Sección 4 del enunciado.

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "Track.h"

using namespace std;

class Playlist {
  
  friend ostream& operator<<(ostream& os, const Playlist& p);

  private:
    // La clase Node es privada: solo la Playlist puede acceder a los nodos.
    struct Node {
        Track track;
        Node* prev;
        Node* next;
        Node(const Track& t) : track(t), prev(nullptr), next(nullptr) {}
    };

    Node* head;      // primer nodo (nullptr si la lista está vacía)
    Node* tail;      // último nodo
    Node* current;   // pista "en reproducción"
    int count;       // número de pistas

    // TODO: métodos privados auxiliares que necesites
    void copy(const Playlist &other);
  public:
    
    Playlist();
    Playlist(const Playlist& other);
    Playlist& operator=(const Playlist& other);
    ~Playlist();

    int size() const;
    bool empty() const;
    bool add(const Track& t);
    bool insertAt(int pos, const Track& t);
    bool removeById(const string& id);
    bool load(const string& filename);
    int merge(const Playlist& other);
    void clear();

    const Track* get(int pos) const;
    const Track* findById(const string& id) const;
    const Track* getCurrent() const;

    bool next();
    bool prev();
    int totalDuration() const;

};

#endif // PLAYLIST_H
