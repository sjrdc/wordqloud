include_directories(
  )

ADD_EXECUTABLE(wordcloud
  canvas.cpp
  colormap.cpp
  main.cpp
  quadnode.cpp
  quadtree.cpp
  word.cpp
  wordlist.cpp  
  )
TARGET_LINK_LIBRARIES(wordcloud
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

install(TARGETS
  wordcloud
  RUNTIME DESTINATION bin/linaccontrolclients
  )

