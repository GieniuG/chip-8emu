#pragma once

class IResettable {
public:
  virtual void Reset() = 0;
  virtual ~IResettable() = default;
};
