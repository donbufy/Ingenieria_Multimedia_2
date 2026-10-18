// Práctica 2 — Reproductor de música
// Plantilla: completa la declaración de la clase Track
// siguiendo la Sección 3 del enunciado.

#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <string>

using namespace std;

class Track {

  friend ostream& operator<<(ostream& os, const Track& t);

  private:
    
    string id;

    string title;

    int duration;

  public:
    
    Track();
    Track(const string& id, const string& title, int duration);
    Track(const Track& other);
    Track& operator=(const Track& other);
    ~Track() = default;

    string getId() const;
    string getTitle() const;
    int getDuration() const;

    string formatDuration() const;

    bool operator ==(const Track& other);
  
};

#endif // TRACK_H
