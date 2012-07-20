

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
  ${OPENCV_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  )

qt4_automoc(
  boundsdialog.h
  boundsdialog.cpp
  colourschemedialog.h
  colourschemedialog.cpp
view.cpp
view.h
  wordqloud.cpp
  wordqloud.h
)
ADD_EXECUTABLE(wordqloud
  boundsdialog.cpp
  colourschemedialog.cpp
  canvas.cpp
  colormap.cpp
  main.cpp
  quadtree.cpp
  quadnode.cpp
view.cpp
  wordqloud.cpp
  word.cpp
  wordlist.cpp
)
TARGET_LINK_LIBRARIES(wordqloud
  ${QT_LIBRARIES}
  ${OpenCV_LIBS}
  )

install(TARGETS
  wordcloud
  RUNTIME DESTINATION bin/linaccontrolclients
  )

