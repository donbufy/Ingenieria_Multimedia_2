// Práctica 1 — Catálogo de Recursos

#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Resource.h"

using namespace std;

class Catalog {
  private:
    vector<Resource> data;

  public:
    Catalog();
    Catalog(const Catalog& other);
    Catalog& operator=(const Catalog& other);
    ~Catalog();
    bool add(const Resource& r);
    Resource findById(const string& id) const;
    bool removeById(const string& id);
    vector<Resource> findByTag(const string& tag) const;
    int size() const;
    bool loadFromFile(const string& filename);
};

#endif // CATALOG_H
