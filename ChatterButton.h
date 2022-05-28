/*!
 * \brief Buttons debounce assistant.
 *
 * Contacts debounce assistant for stable operation of buttons and other hardware triggers.
 *
 * >< works with buttons on GND or Vdd connect
 * >< time tracking of button's states
 * >< can check button's state directly or with callback function
 * >< can button update from main loop or with hardware interrupt
 * >< compatible with Arduino and some many other platforms
 *
 * \author
 *
 * Written by 0xdeb
 *
 * \copyright
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 0xdeb <0xdeb@mail.ru>
 *
 */

#ifndef CHATTERBUTTON_H_
#define CHATTERBUTTON_H_

enum class CButtonWire : bool { GND, Vdd };

/*! BaseChatterButton is common abstract class */

class BaseChatterButton {
protected:
    const unsigned long kChatterTimeMs{20};
 
    bool closeOnGND;

    bool state = false;
    bool hasChange = false;
    long wireAverage = 0;
    unsigned long wireChangeTime = 0;
    unsigned long lastChangeTime = 0;

private:
    unsigned long previousStateDuration;
    
public:
    /**
     * Select button's connection closed on GND or Vdd (positive)
     * also it use to correct connect inverted (normal close) button
     */
    BaseChatterButton(CButtonWire);
    virtual ~BaseChatterButton() = 0;

    /**
     * Return current button state; true - active; once call will reset 'didChange()' flag
     */
    virtual operator bool() = 0;
   
    /**
     * Need call in main loop with set wire/pin state
     */
    virtual void update(bool) = 0;

    /**
     * Indicate for button state changed; will reset after call 'operator bool()'
     */
    virtual bool didChange();

    /**
     * Force set state; didChange() doesn't triggered; 
     * use for forced init etc
     */
    void set(bool);
    
    /**
     * Return duration of current button state; milliseconds
     */
    unsigned long getStateDuration() const;

    /**
     * Return duration of previous button state; milliseconds
     */
    unsigned long getPreviousStateDuration() const;
};

//========================= ChatterButton =============================
/*! ChatterButton is class for button with main loop update */
/*!
 * Button check may be with callback handler or direct poll; see sample code
 */

class ChatterButton : public BaseChatterButton {
public:
    using ButtonStateHandler = void(*)(bool);
private:
    ButtonStateHandler handlerFn = nullptr;

public:
    explicit ChatterButton(CButtonWire connect);
    void setHandler(ButtonStateHandler);
    
    explicit operator bool() override;
    void update(bool) override;
};

//========================= ChatterINTButton ==========================
/*! ChatterINTButton is class for button with hardware interrupt handler */
/*!
 * Button check must be with direct poll; see sample code
 */

class ChatterINTButton : public BaseChatterButton {
private:
    bool isReady;
    bool currentHighLevel;
    
    void updateIfNeeded();
    
public:
    explicit ChatterINTButton(CButtonWire connect);

    bool didChange() override;

    explicit operator bool() override;
    void update(bool) override;
};

#endif /* CHATTERBUTTON_H_ */
