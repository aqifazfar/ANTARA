#pragma once

class Colleague;

class Mediator
{
public:
    virtual void Send(Colleague const &sender, int const &err) = 0;
};

class Colleague
{
protected:
    Mediator &mediator;

public:
    explicit Colleague(Mediator &m) : mediator(m) {}

    virtual void Send_Notification(int const &err)
    {
        mediator.Send(*this, err);
    };

    virtual int Recv_Notification(int const &err)
    {
        return err;
    };
};

template <typename C1, typename C2>
class ConcreteMediator : public Mediator
{

private:
    C1 *collegue1;
    C2 *collegue2;

public:
    ConcreteMediator();

    void Set_Colleague1(C1 &colleague)
    {
        collegue1 = &colleague;
    }

    void Set_Colleague2(C2 &colleague)
    {
        collegue2 = &colleague;
    }

    void Send(Colleague const &sender, int const &err) override
    {
        if (&sender == collegue1)
        {
            collegue2->Recv_Notification(err);
        }
        else
        {
            collegue1->Recv_Notification(err);
        }
    }
};