

ADD_EXECUTABLE(wordcloud
  canvas.cpp
  colormap.cpp
  quadnode.cpp
  quadtree.cpp
  word.cpp
  wordcloud.cpp
  wordlist.cpp  
  )
TARGET_LINK_LIBRARIES(wordcloud
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

include_directories(
  ${CMAKE_BINARY_DIR}/src # for moc files
  )

qt4_automoc(
  wordqloud.cpp
  wordqloud.h  
)
ADD_EXECUTABLE(wordqloud
  main.cpp
  wordqloud.cpp
)
TARGET_LINK_LIBRARIES(wordqloud
  ${QT_LIBRARIES}
  )

install(TARGETS
  wordcloud
  RUNTIME DESTINATION bin/linaccontrolclients
  )

