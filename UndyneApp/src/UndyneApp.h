#ifndef UNDYNE_APP_H
#define UNDYNE_APP_H

#include <UndyneEngine.h>

class UndyneApp final : public UndyneEngine::Application
{
public:
    UndyneApp() = default;
    ~UndyneApp() override = default;

protected: 
    void load() override;
};

#endif
