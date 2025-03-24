import React from "react";
import { Badge } from "../../components/badge";
import { Button } from "../../components/button";
import { Card, CardContent } from "../../components/card";
import { ScrollArea } from "../../components/scroll-area";

// Data for time slots
const timeSlots = [
  "09:00",
  "10:00",
  "11:00",
  "12:00",
  "13:00",
  "14:00",
  "15:00",
  "16:00",
  "17:00",
  "18:00",
  "19:00",
  "20:00",
  "21:00",
  "22:00",
  "23:00",
];

// Data for room columns
const rooms = [
  { id: 1, name: "ПИЛА" },
  { id: 2, name: "Тайны" },
  { id: 3, name: "Цирк" },
  { id: 4, name: "Обряд" },
  { id: 5, name: "Незваные" },
  { id: 6, name: "Гуд" },
  { id: 7, name: "Лофт" },
  { id: 8, name: "Бранчес" },
];

export const A = (): React.ReactElement => {
  return (
    <div className="bg-[#69696980] flex flex-row justify-center w-full min-h-screen">
      <div className="bg-[#69696980] w-[595px] h-[901px] relative">
        <div className="relative w-[390px] h-[855px] mx-auto mt-3.5">
          {/* Top navigation bar */}
          <header className="absolute w-[390px] h-[60px] top-px left-0 bg-white border-b border-[#e7ecf7] flex items-center justify-between px-3 z-10">
            <div className="flex items-center">
              <img className="w-[18px] h-[21px]" alt="Back" src="/icon-1.svg" />

              <div className="flex ml-5 space-x-2">
                <div className="relative">
                  <Button
                    className="w-[34px] h-[34px] p-0 rounded-full hover:bg-transparent"
                  >
                    <img
                      className="w-4 h-[18px]"
                      alt="Notification"
                      src="/icon-4.svg"
                    />
                  </Button>
                  <Badge className="absolute -top-2.5 right-0 bg-[#fcc558] text-white h-[18px] w-[19px] flex items-center justify-center rounded-full p-0">
                    <span className="text-[10.5px]">5</span>
                  </Badge>
                </div>

                <div className="relative">
                  <Button
                    className="w-[34px] h-[34px] p-0 rounded-full hover:bg-transparent"
                  >
                    <img
                      className="w-4 h-[18px]"
                      alt="Messages"
                      src="/icon-3.svg"
                    />
                  </Button>
                  <Badge className="absolute -top-2.5 -right-3 bg-[#26dd7d] text-white h-[18px] w-[31px] flex items-center justify-center rounded-full p-0">
                    <span className="text-[10.5px]">99+</span>
                  </Badge>
                </div>
              </div>
            </div>

            <div className="flex items-center space-x-2">
              <img
                className="w-[18px] h-[18px]"
                alt="Search"
                src="/icon-5.svg"
              />

              <Button className="w-[50px] h-9 bg-[#f4adff] rounded-[50px] border border-solid border-[#d400ff] p-0">
                <div className="w-5 h-5 overflow-hidden">
                  <div className="w-5 h-5 border-0 border-none shadow-[0px_4px_4px_#00000040] bg-[url(/image-117.png)] bg-cover bg-[50%_50%]" />
                </div>
              </Button>

              <Button className="w-9 h-9 p-0 rounded-full hover:bg-transparent">
                <img className="w-[22px] h-[22px]" alt="Menu" src="/svg.svg" />
              </Button>
            </div>
          </header>

          {/* Main frame background */}
          <img
            className="absolute w-[390px] h-[855px] top-0 left-0 z-0"
            alt="Background"
            src="/rectangle-1.svg"
          />

          {/* Date navigation section */}
          <Card className="absolute w-[350px] h-[102px] top-[49px] left-[20px] z-10 border-none shadow-none">
            <CardContent className="p-5">
              <div className="flex justify-center items-center space-x-8">
                <Button className="w-9 h-9 p-0 rounded-full hover:bg-transparent">
                  <img
                    className="w-[7px] h-[18px]"
                    alt="Previous"
                    src="/icon-7.svg"
                  />
                </Button>

                <div className="flex flex-col items-center">
                  <Button
                    className="w-9 h-9 p-0 rounded-full border-[#06adef] hover:bg-transparent"
                  >
                    <img
                      className="w-3.5 h-4"
                      alt="Calendar"
                      src="/icon-6.svg"
                    />
                  </Button>
                  <p className="mt-3 font-bold text-[13px] text-[#0b3052] tracking-[0.16px]">
                    Понедельник, 24-03-2025
                  </p>
                </div>

                <Button className="w-9 h-9 p-0 rounded-full hover:bg-transparent">
                  <img
                    className="w-[7px] h-[18px]"
                    alt="Next"
                    src="/icon.svg"
                  />
                </Button>
              </div>
            </CardContent>
          </Card>

          {/* Calendar grid */}
          <div className="absolute w-[386px] h-[704px] top-[151px] left-0.5 bg-white overflow-hidden border-t border-b border-[#dddddd] z-10">
            {/* Room headers */}
            <div className="flex h-10 border-r border-[#cecece]">
              <div className="w-[50px] h-10 flex-shrink-0"></div>
              {rooms.map((room, index) => (
                <div
                  key={room.id}
                  className={`h-10 bg-white border-l border-[#cecece] flex items-center justify-center ${
                    index === 6 ? "w-14" : index === 7 ? "w-11" : "w-[38px]"
                  }`}
                >
                  <span className="text-[#767676] text-[10.5px] text-center whitespace-nowrap">
                    {room.name}
                  </span>
                </div>
              ))}
            </div>

            {/* Calendar grid with time slots */}
            <ScrollArea className="h-[660px] border-t border-[#cecece]">
              <div className="flex">
                {/* Time column */}
                <div className="w-[51px] border-r border-[#cecece] flex-shrink-0">
                  {timeSlots.map((time, index) => (
                    <div
                      key={index}
                      className="h-11 bg-white border-b border-[#cecece] relative"
                    >
                      <div className="absolute top-[15px] left-[11px] text-[#999999] text-[10.5px] text-center whitespace-nowrap">
                        {time}
                      </div>
                      <div className="absolute w-[9999px] h-0.5 top-[42px] left-0 border-b-2 border-dashed border-[#0000001a]" />
                    </div>
                  ))}
                </div>

                {/* Room columns */}
                {rooms.map((room, roomIndex) => (
                  <div
                    key={room.id}
                    className={`border-r border-[#cecece] relative ${
                      roomIndex === 6
                        ? "w-14"
                        : roomIndex === 7
                          ? "w-[52px]"
                          : "w-[38px]"
                    }`}
                  >
                    {timeSlots.map((_, timeIndex) => (
                      <div
                        key={timeIndex}
                        className="h-11 relative bg-white border-b border-[#cecece]"
                      >
                        <div className="absolute w-[9999px] h-0.5 top-[42px] left-0 border-b-2 border-dashed border-[#0000001a]" />
                        {/* Half-hour divider */}
                        <div className="absolute w-full h-[1px] top-[21px] left-0 border-t border-[#cecece]" />
                      </div>
                    ))}
                  </div>
                ))}
              </div>
            </ScrollArea>
          </div>
        </div>
      </div>
    </div>
  );
};