#ifndef					__FRAME_INFO_HPP__
# define				__FRAME_INFO_HPP__

struct  FrameInfo
{
  std::string				path;
  unsigned int				number;
  time_t				timeInfos;
  bool					test;

  FrameInfo(const std::string &aPath, unsigned int aNumber, bool aTest)
    : path(aPath)
    , number(aNumber)
    , timeInfos(time(NULL))
    , test(aTest)
  {}
};

#endif					//FRAME_INFO_HPP__
