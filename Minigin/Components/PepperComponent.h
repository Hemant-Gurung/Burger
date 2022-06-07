#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include "Observer.h"
class PepperComponent :
    public dae::BaseComponent,public dae::Observer
{
public:
    PepperComponent(std::shared_ptr<dae::GameObject>&, std::shared_ptr<dae::TextComponent> text);
    ~PepperComponent() = default;

    PepperComponent(const PepperComponent & other) = delete;
    PepperComponent(PepperComponent && other) = delete;
    PepperComponent& operator=(const PepperComponent & other) = delete;
    PepperComponent& operator=(PepperComponent && other) = delete;

    void update(float) override;
    void Render() const override;
    void SetPosition(float x, float y, float z);
    //observer
    void OnNotify(const dae::BaseComponent&, dae::EVENT&);
private:
    std::shared_ptr<dae::TextComponent> m_TextComponent;
  
};

