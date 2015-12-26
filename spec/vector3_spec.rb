require './lib/sfml/rbsfml.so'

describe SFML::Vector3 do
  describe "in creation" do
    context "given no arguments" do
      obj = SFML::Vector3.new
      it "vector attributes should be zero" do
        expect(obj.x).to be(0)
        expect(obj.y).to be(0)
        expect(obj.z).to be(0)
      end

      it "should be equal to the zero vector" do
        expect(obj == SFML::Vector3::Zero).to be_truthy
      end

      it "should be equal an array of zeros" do
        expect(obj == [0, 0, 0]).to be_truthy
      end
    end

    context "given fixnum arguments" do
      obj = SFML::Vector3.new(3, 8, 9)
      it "vector attributes should be equal to given values" do
        expect(obj.x).to eql(3)
        expect(obj.y).to eql(8)
        expect(obj.z).to eql(9)
      end

      it "vector attributes should be of fixnum type" do
        expect(obj.x).to be_kind_of(Fixnum)
        expect(obj.y).to be_kind_of(Fixnum)
        expect(obj.z).to be_kind_of(Fixnum)
      end
    end

    context "given float arguments" do
      obj = SFML::Vector3.new(3.0, 8.0, 9.0)
      it "vector attributes should be equal to given values" do
        expect(obj.x).to eql(3.0)
        expect(obj.y).to eql(8.0)
        expect(obj.z).to eql(9.0)
      end

      it "vector attributes should be of float type" do
        expect(obj.x).to be_kind_of(Float)
        expect(obj.y).to be_kind_of(Float)
        expect(obj.z).to be_kind_of(Float)
      end
    end
  end

  describe "in math" do
    context "given same type vectors" do
      vec1 = SFML::Vector3.new(3, 9, 2)
      vec2 = SFML::Vector3.new(7, 4, 5)
      it "addition will produce expected result" do
        expect(vec1 + vec2).to be(SFML::Vector3.new(10, 13, 7))
      end

      it "subtraction will produce expected result" do
        expect(vec1 - vec2).to be(SFML::Vector3.new(-4, 5, -3))
      end

      it "multiplication will produce expected result" do
        expect(vec1 * vec2).to be(SFML::Vector3.new(21, 36, 10))
      end

      it "division will produce expected result" do
        expect(vec1 / vec2).to be(SFML::Vector3.new(0, 2, 0))
      end
    end

    context "given different type vectors" do
      vec1 = SFML::Vector3.new(10.0, 12.0, 6.0)
      vec2 = SFML::Vector3.new(2, 5, 4)
      it "addition will produce expected result" do
        expect(vec1 + vec2).to be(SFML::Vector3.new(12.0, 17.0, 10.0))
      end

      it "subtraction will produce expected result" do
        expect(vec1 - vec2).to be(SFML::Vector3.new(8.0, 7.0, 2.0))
      end

      it "multiplication will produce expected result" do
        expect(vec1 * vec2).to be(SFML::Vector3.new(20.0, 60.0, 24.0))
      end

      it "division will produce expected result" do
        expect(vec1 / vec2).to be(SFML::Vector3.new(5.0, 2.4, 1.5))
      end
    end
  end
end